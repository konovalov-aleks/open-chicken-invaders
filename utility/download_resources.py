#!/usr/bin/env python3

import ssl
import os
import json
import urllib.request
from urllib.parse import urlencode
from hashlib import md5

TARGET_FILE = 'ChickenInvaders2.dat'
EXPECTED_MD5 = '0a4ba26b610262697222cfaa212e171c'

BASE_URL = 'https://cloud-api.yandex.net/v1/disk/public/resources/download?'
PUBLIC_KEY = 'https://disk.yandex.ru/d/0gZmYHpGPAaLZ'

ssl._create_default_https_context = ssl._create_unverified_context


def check_hash(data):
    return md5(data).hexdigest() == EXPECTED_MD5


def main():
    print('downloading ChickenInvaders2.dat...')
    if os.path.exists(TARGET_FILE):
        try:
            with open(TARGET_FILE, 'rb') as f:
                if check_hash(f.read()):
                    # the file exists, hash is correct - skip downloading
                    print(' - skipped (already exists)')
                    return
        except OSError:
            pass

    # get the download link
    final_url = BASE_URL + urlencode({'public_key': PUBLIC_KEY})
    with urllib.request.urlopen(final_url) as response:
        download_url = json.load(response)['href']

    # download the file
    with urllib.request.urlopen(download_url) as response:
        data = response.read()
        if not check_hash(data):
            raise Exception('Unable to download ChickenInvaders2.dat: wrong MD5 hash')
        with open('ChickenInvaders2.dat', 'wb') as f:
            f.write(data)
    print(' - done')


if __name__ == '__main__':
    main()
