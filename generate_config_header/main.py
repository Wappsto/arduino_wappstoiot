#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import json
import pathlib
import uuid
import getpass

import requests

from jinja2 import Environment, FileSystemLoader


debug = False

template_loc = pathlib.Path(__file__).parent / pathlib.Path('templates')

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
    "dev": "dev.wappsto.com",
    "qa": "qa.wappsto.com",
    "staging": "stagning.wappsto.com",
    "prod": "wappsto.com",
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


def _log_request_error(data):
    if debug:
        print("Sent data     :")
        print(" - URL         : {}".format(data.request.url))
        print(" - Headers     : {}".format(data.request.headers))
        print(" - Request Body: {}".format(
            json.dumps(data.request.body, indent=4, sort_keys=True))
        )

        print("")
        print("")

        print("Received data :")
        print(" - URL         : {}".format(data.url))
        print(" - Headers     : {}".format(data.headers))
        print(" - Status code : {}".format(data.status_code))
        try:
            print(" - Request Body: {}".format(
                json.dumps(json.loads(data.text), indent=4, sort_keys=True))
            )
        except (AttributeError, json.JSONDecodeError):
            pass
    try:
        err = json.loads(data.text)
    except Exception:
        err = data.text
    else:
        err = err.get('message', f"Unknown Error: http error: {data.status_code}")
    print(f"\t{err}")
    exit(-2)


def start_session(base_url, username, password):
    session_json = {
        "username": username,
        "password": password,
        "remember_me": False
    }

    url = f"https://{base_url}/services/session"
    headers = {"Content-type": "application/json"}
    data = json.dumps(session_json)

    rdata = requests.post(
        url=url,
        headers=headers,
        data=data
    )
    if rdata.status_code >= 300:
        print("\nAn error occurred during login:")
        _log_request_error(rdata)

    rjson = json.loads(rdata.text)

    return rjson["meta"]["id"]


def create_network(
    session,
    base_url,
    # network_uuid=None,
    dry_run=False
):
    # Should take use of the more general functions.
    request = {}

    url = f"https://{base_url}/services/2.1/creator"
    headers = {
        "Content-type": "application/json",
        "X-session": str(session)
    }
    data = json.dumps(request)

    if not dry_run:
        rdata = requests.post(
            url=url,
            headers=headers,
            data=data
        )

        if rdata.status_code >= 300:
            print("\nAn error occurred during Certificate retrieval:")
            _log_request_error(rdata)

        rjson = json.loads(rdata.text)
    else:
        rjson = {
            'network': {'id': 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx'},
            'ca': "NOTHING",
            'certificate': "NOTHING",
            'private_key': "NOTHING",
            'meta': {'id': 'xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx'}
        }

    print(f"\nCertificate generated for new network:\t{rjson['network']['id']}")

    return rjson


def get_network(session, base_url, network_uuid):
    url = f"https://{base_url}/services/2.1/creator?this_network.id={network_uuid}"
    headers = {
        "Content-type": "application/json",
        "X-session": str(session)
    }

    rdata = requests.get(
        url=url,
        headers=headers
    )

    if rdata.status_code >= 300:
        print("\nAn error occurred during Certificate retrieval:")
        _log_request_error(rdata)
    data = json.loads(rdata.text)

    if not data['id']:
        print(f"{data['message']}")
        exit(-3)
    creator_id = data['id'][0]
    url = f"https://{base_url}/services/2.1/creator/{creator_id}"

    rdata = requests.get(
        url=url,
        headers=headers
    )

    if rdata.status_code >= 300:
        print("\nAn error occurred during Certificate retrieval:")
        _log_request_error(rdata)

    rjson = json.loads(rdata.text)

    print(f"\nCertificate retrieved for network:\t{rjson['network']['id']}")

    return rjson


def gen_file(location, creator, args):
    file_name = FileType2file[args.type]
    file_out = location / file_name
    file_data = Environment(
        loader=FileSystemLoader(
            searchpath=template_loc
        )
    ).get_template(
        name=str(file_name)
    ).render(
        network_uuid=creator['network']['id'],
        creator_uuid=creator['meta']['id'],
        ca=creator["ca"].encode("unicode_escape").decode("utf-8"),
        crt=creator["certificate"].encode("unicode_escape").decode("utf-8"),
        key=creator["private_key"].encode("unicode_escape").decode("utf-8"),
        base_url=f"collector.{wappstoUrl[args.env]}",
    )

    if not args.dry_run:
        location.mkdir(exist_ok=True)
        try:
            with file_out.open("w") as file:
                file.write(file_data)
        except Exception as err:
            print("\nAn error occurred while saving Certificates:")
            print(f"\t{err}")
            print("\nWhen fixed you can recreate the certificate file with the --recreate option.")
            exit(-3)
    print(f"\nLocation of generated {args.type} header:\t{file_out.absolute()}")


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--dry_run",
        action='store_true',
        help="Run the Script, without making the files & folders.",
    )
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
        creator = create_network(session=session, base_url=wappstoUrl[args.env], dry_run=args.dry_run)
    else:
        creator = get_network(
            session=session,
            base_url=wappstoUrl[args.env],
            network_uuid=args.recreate,
        )

    gen_file(args.path, creator, args)

    print("\nEnjoy...")
