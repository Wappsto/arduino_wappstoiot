#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import json
import pathlib
import uuid
import getpass

import urllib.parse
import urllib.request
import urllib

from jinja2 import Environment, FileSystemLoader


debug = False

wappstoEnv = [
    "dev",
    "qa",
    "stagning",
    "prod",
]


wappstoPort = {
    "dev": 52005,
    "qa": 53005,
    "stagning": 54005,
    "prod": 443
}

wappstoUrl = {
    "dev": "https://dev.wappsto.com",
    "qa": "https://qa.wappsto.com",
    "staging": "https://stagning.wappsto.com",
    "prod": "https://wappsto.com",
}

FileType = [
    "c++",
    "c",
    "arduino",
]

FileType2file = {
    "c++": pathlib.Path("wappsto_config.h"),
    "c": pathlib.Path("wappsto_config.h"),
    "arduino": pathlib.Path("wappsto_config.h"),
}


def post(*, url, data, **kwargs):
    """
    URL-POST to the given Link.

    To change info.

    Args:
        url: The url to send POST request to.
        data: The Data, that are sent.

    Returns:
        The response from the POST request.

    Raises:
        HTTP Error, from urllib.error import HTTPError
    """
    return _send(url=url, data=data, method="POST", **kwargs)


def get(*, url, **kwargs):
    """
    URL-GET to the give URL.

    Receive Info.

    Args:
        url: The url to send GET request to.

    Returns:
        The response from the GET request.

    Raises:
        HTTP Error, from urllib.error import HTTPError
    """
    return _send(url=url, **kwargs)


def _send(url, data=None, header={}, **kwargs):
    """
    Standardize URL-open request.

    Args:
        data: (Optional) The Data to be sent.
        method: (Optional) The HTTP method. (default: GET)
        **Kwargs: sent on to urllib.request.Request

    Returns:
        The response from the PUT request.

    Raises:
        HTTP Error, from urllib.error import HTTPError
    """
    kwargs["data"] = convert_data(data)
    req = urllib.request.Request(url=url, **kwargs)
    for keys in header.keys():
        req.add_header(key=keys, val=header[keys])
    with urllib.request.urlopen(req) as conn:
        return conn.read().decode()


def convert_data(data):
    """Convert the data, for use with the urllib library."""
    if isinstance(data, str):
        return str.encode(data)
    elif data:
        return urllib.parse.urlencode(data).encode()


def _log_request_error(url, data, err, headers):
    if debug:
        print("Sendt data    :")
        print(" - URL         : {}".format(url))
        print(" - Headers     : {}".format(headers))
        print(" - Request Body: {}".format(
            json.dumps(data, indent=4, sort_keys=True))
        )

        print("")
        print("")

        print("Received data :")
        print(" - URL         : {}".format(url))
        print(" - Headers     : {}".format(headers))
        print(" - Status code : {}".format(err.code))
        try:
            print(" - Request Body: {}".format(
                json.dumps(json.loads(err), indent=4, sort_keys=True))
            )
        except (AttributeError, json.JSONDecodeError):
            pass
    print(f"{err.msg}")
    exit(-2)


def start_session(base_url, username, password):
    session_json = {
        "username": username,
        "password": password,
        "remember_me": False
    }

    url = f"{base_url}/services/session"
    headers = {"Content-type": "application/json"}
    data = json.dumps(session_json)

    try:
        rdata = post(
            url=url,
            headers=headers,
            data=data
        )
    except urllib.error.HTTPError as err:
        _log_request_error(url=url, data=data, err=err, headers=headers)

    rjson = json.loads(rdata)

    return rjson["meta"]["id"]


def create_network(
    session,
    base_url,
    # network_uuid=None,
    product=None,
    test_mode=False,
    reset_manufacturer=False,
    manufacturer_as_owner=True
):
    # Should take use of the more general functions.
    request = {
    }
    # if network_uuid:
    #     request["network"] = {"id": uuid}
    if product:
        request["product"] = product

    if test_mode:
        request["test_mode"] = True

    if reset_manufacturer:
        request["factory_reset"] = {"reset_manufacturer": True}

    request['manufacturer_as_owner'] = manufacturer_as_owner

    url = f"{base_url}/services/2.1/creator"
    headers = {
        "Content-type": "application/json",
        "X-session": str(session)
    }

    data = json.dumps(request)

    try:
        rdata = post(
            url=url,
            headers=headers,
            data=data
        )
    except urllib.error.HTTPError as err:
        _log_request_error(url=url, data=data, err=err, headers=headers)

    rjson = json.loads(rdata)

    return rjson


def get_network(session, base_url, network_uuid):
    url = f"{base_url}/services/2.1/creator?this_network.id={network_uuid}"
    headers = {
        "Content-type": "application/json",
        "X-session": str(session)
    }
    try:
        rdata = get(
            url=url,
            headers=headers
        )
    except urllib.error.HTTPError as err:
        _log_request_error(url=url, data=None, err=err, headers=headers)

    data = json.loads(rdata)

    if not data['id']:
        print(f"{data['message']}")
        exit(-3)
    creator_id = data['id'][0]
    url = f"{base_url}/services/2.1/creator/{creator_id}"

    try:
        rdata = get(
            url=url,
            headers=headers
        )
    except urllib.error.HTTPError as err:
        _log_request_error(url=url, data=None, err=err, headers=headers)

    return json.loads(rdata)


def gen_file(location, creator, args):
    file_name = FileType2file[args.type]
    file_data = Environment(
        loader=FileSystemLoader(
            searchpath='templates'
        )
    ).get_template(
        name=str(file_name)
    ).render(
        network_uuid=creator['network']['id'],
        #creator_uuid=creator['meta']['id'], 
        ca=creator["ca"].encode("unicode_escape").decode("utf-8"),
        crt=creator["certificate"].encode("unicode_escape").decode("utf-8"),
        key=creator["private_key"].encode("unicode_escape").decode("utf-8"),
        #base_url=wappstoUrl[args.env],
    )
    with open(location / file_name, "w") as file:
        file.write(file_data)


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--env",
        type=str,
        choices=wappstoEnv,
        default="prod",
        help="Wappsto environment."
    )
    parser.add_argument(
        "--token",
        type=uuid.UUID,
        help="The Session Token. If not given, you are prompted to login."
    )
    parser.add_argument(
        "--path",
        type=pathlib.Path,
        default=".",
        help="The location to which save the config file."
    )
    parser.add_argument(
        "--type",
        type=str,
        choices=FileType,
        help="Which programming language the config should be.",
        required=True
    )
    parser.add_argument(
        "--recreate",
        type=uuid.UUID,
        help="Recreate Config file, for given network UUID. (Overwrites existent)"
    )
    parser.add_argument(
        "--debug",
        action='store_true',
        help="Make the operation more talkative",
    )

    args = parser.parse_args()

    debug = args.debug if args.debug else False

    if not args.token:
        session = start_session(
            base_url=wappstoUrl[args.env],
            username=input("Wappsto Username: "),
            password=getpass.getpass(prompt="Wappsto Password: "),
        )
    else:
        session = args.token
    if not args.recreate:
        creator = create_network(session=session, base_url=wappstoUrl[args.env])
    else:
        creator = get_network(
            session=session,
            base_url=wappstoUrl[args.env],
            network_uuid=args.recreate,
        )

    args.path.mkdir(exist_ok=True)

    gen_file(args.path, creator, args)

    print(f"\nNew network: {creator['network']['id']}")
    print(f"Config saved at: {args.path}")
