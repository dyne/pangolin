<p align="center">
  <a href="https://www.dyne.org">
    <img alt="Pangolin" src="./img/pangolin.svg" width="150" />
  </a>
</p>

<h1 align="center">
  The Secret Pangolin Code</br>
  <sub>Fastest Proximity Tracing in the West (FPTW)</sub>
</h1>

<p align="center">
  <a href="https://dyne.org">
    <img src="https://img.shields.io/badge/%3C%2F%3E%20with%20%E2%9D%A4%20by-Dyne.org-blue.svg" alt="Dyne.org">
  </a>
</p>

<br><br>

<h4 align="center">
  <a href="#-install">💾 Install</a>
  <span> • </span>
  <a href="#-quick-start">🎮 Quick start</a>
  <span> • </span>
  <a href="#-api">🐝 API</a>
  <span> • </span>
  <a href="#-testing">📋 Testing</a>
  <span> • </span>
  <a href="#-acknowledgements">😍 Acknowledgements</a>
  <span> • </span>
  <a href="#-links">🌐 Links</a>
  <span> • </span>
  <a href="#-contributing">👤 Contributing</a>
  <span> • </span>
  <a href="#-license">💼 License</a>
</h4>

<br><br>

This is a library implementing "proximity tracing" protocols for privacy-preserving contact tracing. 

The Secret Pangolin Code has the ambition to run at blazing fast speeds while being easy to maintain and fully compliant with FIPS.

This code is the Fastest Proximity Tracing in the West, until proven wrong of course! and developed with the _embedded first_ mindset.

At this moment the first and only protocol implemented is [DP3T](https://github.com/dp-3T/documents)

Pangolin is crafted with care by developers of the [DECODE project](https://decodeproject.eu) about data-ownership and [technological sovereignty](https://www.youtube.com/watch?v=RvBRbwBm_nQ). Our effort is that of improving people's awareness of how their data is processed by algorithms, as well facilitate the work of developers to create along [privacy by design principles](https://decodeproject.eu/publications/privacy-design-strategies-decode-architecture) using algorithms that can be deployed in any situation without any change.


<details id="toc">
 <summary><strong>🚩 Table of Contents</strong> (click to expand)</summary>

* [Install](#-install)
* [Quick start](#-quick-start)
* [API](#-api)
* [Testing](#-testing)
* [Troubleshooting & debugging](#-troubleshooting--debugging)
* [Acknowledgements](#-acknowledgements)
* [Links](#-links)
* [Contributing](#-contributing)
* [License](#-license)
</details>

---
## 💾 Install

Simply run the default target of `Makefile`

```bash
make
```

---
## 🎮 Quick start

To see Pangolin in action your best move is to run the tests like

```bash
make check
```

---
## 🐝 API

Look a the [header
file](https://github.com/dyne/pangolin/blob/master/src/dp3t.h) to see
the current API, more documentation will come soon.

Functions:
```c
void renew_key(sk_t dest, sk_t src);

int32_t generate_beacons(beacons_t *beacons, uint32_t max_beacons,
                         const sk_t oldest_sk, const uint32_t day, const uint32_t ttl,
                         const char *bk, uint32_t bklen);

int32_t match_positive(matches_t *matches, uint32_t max_matches,
                       const sk_t positive, const contacts_t *contacts);
					   
```

Data structures:
```c
// simple offset structure of num elements sized EPHID_LEN bytes
typedef struct {
	uint32_t epochs;           //< data length (capacity) provided by caller
	char     broadcast[32];   //< broadcast key
	uint32_t broadcast_len;  //< broadcast key length
	beacon_t ephids[0];    //< data offset provided by caller
} beacons_t;

typedef struct __attribute__((packed)) {
	uint8_t day;
	uint8_t epoch;
	uint8_t rssi;
	uint8_t reserved;
	uint8_t data[16];
} contact_t;

typedef struct {
	uint32_t count;             //< number of ephids stored
	contact_t *ephids;         //< array of ephids
	uint32_t epochs;          //< how many epochs in a day
	char     broadcast[32];  //< broadcast key
	uint32_t broadcast_len; //< broadcast key length
} contacts_t; // always const

// struct filled with match_positive results, it does not use more
// memory but returns pointers to contact_t data passed in from
// contacts_t
typedef struct {
	uint32_t count;           //< number of ephids stored
	contact_t *ephids[0];     //< array of pointers to contacts found
} matches_t;
```

## 📋 Testing

Running the test with

```bash
make check
```

will test the build against [DP3T vectors published here](https://github.com/DP-3T/documents/issues/62).

To ease the maintainance, production and comparison of test vectors one can also use our other software [Zenroom](https://zenroom.org) which also includes a [DP3T implemetation in Zencode](https://medium.com/@jaromil/decentralized-privacy-preserving-proximity-tracing-cryptography-made-easy-af0a6ae48640)

---
## 🐛 Troubleshooting & debugging

> Nothing to report as per now

If you find any problem or suspicious behaviours please [open an issue](../../issues)

---
## 😍 Acknowledgements

Copyright © 2020 by [Dyne.org](https://www.dyne.org) foundation, Amsterdam.

**The Secret Pangolin Code is licensed as AGPLv3; we are open to grant license exceptions for specific needs.**

Designed, written and maintained by Daniele Lacamera & Denis "Jaromil" Roio.

with contributions by Puria Nafisi Azizi.

This software relies on the awesome **wolfSSL/wolfCrypt** library.

The temporary Icon is made by [Freepik](https://www.flaticon.com/authors/freepik) from [www.flaticon.com](http://www.flaticon.com/)

The name of this project is a tribute to the [Secret Rabbit Code](http://www.mega-nerd.com/SRC/) by Erik de Castro Lopo, a seminal project in the Linux Audio scene.

---
## 🌐 Links

The Secret Pangolin Code is used by:
- [DECODE Proximity HW](https://github.com/dyne/decode-proximity-hw): our own implementation for NRF52DK, Arduino Nano 33 IoT, Pinetime etc.
- [ESP32-DP3T](https://github.com/chriamue/esp32-dp3t): third party, beware use of mbedTLS may break FIPS compliancy at the moment

More links:
- [Decentralized Privacy-Preserving Proximity Tracing (DP3T)](https://github.com/DP-3T/documents)
- [Dyne.org foundation (Amsterdam)](https://dyne.org/)
- [DECODE project on data sovereignty](https://decodeproject.eu)
- [Zenroom crypto VM](https://zenroom.org)

---
## 👥 Contributing

Please first take a look at the [Dyne.org - Contributor License Agreement](CONTRIBUTING.md) then

1.  🔀 [FORK IT](../../fork)
2.  Create your feature branch `git checkout -b feature/branch`
3.  Commit your changes `git commit -am 'Add some fooBar'`
4.  Push to the branch `git push origin feature/branch`
5.  Create a new Pull Request
6.  🙏 Thank you

---
## 💼 Disclaimer
    The Secret Pangolin Code, aka the Fastest Proximity Tracing in the West (FPTW)
    Copyright (c) 2020 Dyne.org foundation, Amsterdam

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

**[🔝 back to top](#toc)**
