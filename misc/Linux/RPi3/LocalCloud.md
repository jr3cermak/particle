# Particle "The Local Cloud"

We walk a fine line between some original instructions and some
modern instructions.

## References

* [Original](https://github.com/spark/spark-server/blob/master/raspberryPi.md) instructions
* [Forum](https://community.particle.io/t/tutorial-local-cloud-1st-time-instructions-01-oct-15/5589) instructions
* [Modern](https://github.com/Brewskey/spark-server) instructions

# Installation

We assume you have root access.  If you are doing things via sudo, that
is possible.  Prefix commands below with `sudo`.

# Be sure the Raspberry Pi 3 (RPi3) is up to date

```
apt-get update
apt-get upgrade
```

# Install Node.js

[Reference](https://nodejs.org/en/download/package-manager/#debian-and-ubuntu-based-linux-distributions)

```
curl -sL https://deb.nodesource.com/setup_6.x | bash -
apt-get install -y nodejs
```

If you plan to compile and install native addons from npm, you may
need to install build tools:

```
apt-get install -y build-essential
```

# Install DFU-Util

```
apt-get install -y libusb-1.0-0-dev
wget http://dfu-util.gnumonks.org/releases/dfu-util-0.7.tar.gz
tar xvf dfu-util-0.7.tar.gz
cd dfu-util-0.7
./configure
make
make install
```

## udev

To assist with allowing regular users to access the USB port, you
can try to use this udev configuration file.   Place file in
`/etc/udev/rules.d` and reboot.

[50-particle.rules](https://raw.githubusercontent.com/jr3cermak/particle/master/misc/Linux/50-particle.rules)

# Install the Particle-CLI

```
npm install -g particle-cli --unsafe-perm
```

Result:

```
/usr/lib
└─┬ particle-cli@1.23.1 
  ├─┬ babel-polyfill@6.23.0 
  │ ├── core-js@2.5.0 
  │ └── regenerator-runtime@0.10.5 
  ├── babel-runtime@6.25.0 
  ├─┬ binary-version-reader@0.4.0 
  │ ├── buffer-crc32@0.2.13 
  │ └── h5.buffers@0.1.1 
  ├─┬ chalk@1.1.3 
  │ ├── ansi-styles@2.2.1 
  │ ├── escape-string-regexp@1.0.5 
  │ ├─┬ has-ansi@2.0.0 
  │ │ └── ansi-regex@2.1.1 
  │ ├── strip-ansi@3.0.1 
  │ └── supports-color@2.0.0 
  ├── cli-spinner@0.2.6 
  ├─┬ cli-table@0.3.1 
  │ └── colors@1.0.3 
  ├─┬ es6-promisify@4.1.0 
  │ └── es6-promise@3.3.1 
  ├─┬ form-data@1.0.0-relativepath.2 
  │ ├── async@1.5.2 
  │ ├─┬ combined-stream@1.0.5 
  │ │ └── delayed-stream@1.0.0 
  │ └─┬ mime-types@2.1.16 
  │   └── mime-db@1.29.0 
  ├─┬ glob@5.0.15 
  │ ├─┬ inflight@1.0.6 
  │ │ └── wrappy@1.0.2 
  │ ├── inherits@2.0.3 
  │ ├─┬ minimatch@3.0.4 
  │ │ └─┬ brace-expansion@1.1.8 
  │ │   ├── balanced-match@1.0.0 
  │ │   └── concat-map@0.0.1 
  │ ├── once@1.4.0 
  │ └── path-is-absolute@1.0.1 
  ├─┬ handlebars@4.0.10 
  │ ├─┬ optimist@0.6.1 
  │ │ ├── minimist@0.0.10 
  │ │ └── wordwrap@0.0.3 
  │ ├─┬ source-map@0.4.4 
  │ │ └── amdefine@1.0.1 
  │ └─┬ uglify-js@2.8.29 
  │   ├── source-map@0.5.6 
  │   ├── uglify-to-browserify@1.0.2 
  │   └─┬ yargs@3.10.0 
  │     ├── camelcase@1.2.1 
  │     ├─┬ cliui@2.1.0 
  │     │ ├─┬ center-align@0.1.3 
  │     │ │ ├─┬ align-text@0.1.4 
  │     │ │ │ ├─┬ kind-of@3.2.2 
  │     │ │ │ │ └── is-buffer@1.1.5 
  │     │ │ │ ├── longest@1.0.1 
  │     │ │ │ └── repeat-string@1.6.1 
  │     │ │ └── lazy-cache@1.0.4 
  │     │ ├── right-align@0.1.3 
  │     │ └── wordwrap@0.0.2 
  │     └── window-size@0.1.0 
  ├── hogan.js@2.0.0 
  ├─┬ inquirer@3.2.1 
  │ ├── ansi-escapes@2.0.0 
  │ ├─┬ chalk@2.0.1 
  │ │ ├─┬ ansi-styles@3.2.0 
  │ │ │ └─┬ color-convert@1.9.0 
  │ │ │   └── color-name@1.1.3 
  │ │ └─┬ supports-color@4.2.1 
  │ │   └── has-flag@2.0.0 
  │ ├─┬ cli-cursor@2.1.0 
  │ │ └─┬ restore-cursor@2.0.0 
  │ │   ├─┬ onetime@2.0.1 
  │ │   │ └── mimic-fn@1.1.0 
  │ │   └── signal-exit@3.0.2 
  │ ├── cli-width@2.1.0 
  │ ├─┬ external-editor@2.0.4 
  │ │ ├── iconv-lite@0.4.18 
  │ │ └── jschardet@1.5.0 
  │ ├── figures@2.0.0 
  │ ├── lodash@4.17.4 
  │ ├── mute-stream@0.0.7 
  │ ├─┬ run-async@2.3.0 
  │ │ └── is-promise@2.1.0 
  │ ├── rx-lite@4.0.8 
  │ ├── rx-lite-aggregates@4.0.8 
  │ ├─┬ string-width@2.1.1 
  │ │ ├── is-fullwidth-code-point@2.0.0 
  │ │ └─┬ strip-ansi@4.0.0 
  │ │   └── ansi-regex@3.0.0 
  │ ├─┬ strip-ansi@4.0.0 
  │ │ └── ansi-regex@3.0.0 
  │ └── through@2.3.8 
  ├─┬ latest-version@2.0.0 
  │ └─┬ package-json@2.4.0 
  │   ├─┬ got@5.7.1 
  │   │ ├─┬ create-error-class@3.0.2 
  │   │ │ └── capture-stack-trace@1.0.0 
  │   │ ├── duplexer2@0.1.4 
  │   │ ├── is-redirect@1.0.0 
  │   │ ├── is-retry-allowed@1.1.0 
  │   │ ├── is-stream@1.1.0 
  │   │ ├── lowercase-keys@1.0.0 
  │   │ ├── node-status-codes@1.0.0 
  │   │ ├─┬ parse-json@2.2.0 
  │   │ │ └─┬ error-ex@1.3.1 
  │   │ │   └── is-arrayish@0.2.1 
  │   │ ├── read-all-stream@3.1.0 
  │   │ ├── timed-out@3.1.3 
  │   │ ├── unzip-response@1.0.2 
  │   │ └─┬ url-parse-lax@1.0.0 
  │   │   └── prepend-http@1.0.4 
  │   ├─┬ registry-auth-token@3.3.1 
  │   │ └─┬ rc@1.2.1 
  │   │   ├── ini@1.3.4 
  │   │   ├── minimist@1.2.0 
  │   │   └── strip-json-comments@2.0.1 
  │   └── registry-url@3.1.0 
  ├── lodash@3.10.1 
  ├── moment@2.18.1 
  ├─┬ node-wifiscanner2@1.2.0 
  │ └─┬ os-locale@1.4.0 
  │   └─┬ lcid@1.0.0 
  │     └── invert-kv@1.0.0 
  ├─┬ particle-api-js@6.5.0 
  │ ├── form-data@1.0.0-rc4 
  │ ├─┬ stream-http@2.2.1 
  │ │ ├── builtin-status-codes@2.0.0 
  │ │ └── to-arraybuffer@1.0.1 
  │ ├─┬ superagent@2.3.0 
  │ │ ├── component-emitter@1.2.1 
  │ │ ├── cookiejar@2.1.1 
  │ │ ├── form-data@1.0.0-rc4 
  │ │ ├── formidable@1.1.1 
  │ │ ├── methods@1.1.2 
  │ │ ├── mime@1.3.6 
  │ │ ├── qs@6.5.0 
  │ │ └─┬ readable-stream@2.3.3 
  │ │   ├── isarray@1.0.0 
  │ │   ├── process-nextick-args@1.0.7 
  │ │   ├── string_decoder@1.0.3 
  │ │   └── util-deprecate@1.0.2 
  │ └── superagent-prefix@0.0.2 
  ├─┬ particle-commands@0.2.11 
  │ ├─┬ mkdirp@0.5.1 
  │ │ └── minimist@0.0.8 
  │ ├── underscore@1.8.3 
  │ └── winreg@1.2.4 
  ├─┬ particle-library-manager@0.1.12 
  │ ├── app-root-path@2.0.1 
  │ ├─┬ async@2.5.0 
  │ │ └── lodash@4.17.4 
  │ ├── babel-plugin-array-includes@2.0.3 
  │ ├── babel-plugin-transform-runtime@6.23.0 
  │ ├─┬ es6-promisify@5.0.0 
  │ │ └── es6-promise@4.1.1 
  │ ├─┬ fs-extra@2.1.2 
  │ │ ├── graceful-fs@4.1.11 
  │ │ └── jsonfile@2.4.0 
  │ ├─┬ gunzip-maybe@1.4.1 
  │ │ ├─┬ browserify-zlib@0.1.4 
  │ │ │ └── pako@0.2.9 
  │ │ ├── is-deflate@1.0.0 
  │ │ ├── is-gzip@1.0.0 
  │ │ ├─┬ peek-stream@1.1.2 
  │ │ │ └─┬ duplexify@3.5.1 
  │ │ │   └── stream-shift@1.0.0 
  │ │ └── pumpify@1.3.5 
  │ ├── klaw@1.3.1 
  │ ├─┬ properties-parser@0.3.1 
  │ │ └── string.prototype.codepointat@0.2.0 
  │ ├─┬ superagent@3.5.2 
  │ │ └─┬ form-data@2.2.0 
  │ │   └── asynckit@0.4.0 
  │ ├─┬ tar-fs@1.15.3 
  │ │ ├── chownr@1.0.1 
  │ │ └── pump@1.0.2 
  │ ├─┬ tar-stream@1.5.4 
  │ │ ├── bl@1.2.1 
  │ │ └── end-of-stream@1.4.0 
  │ ├── tmp@0.0.31 
  │ └─┬ verror@1.10.0 
  │   ├── assert-plus@1.0.0 
  │   ├── core-util-is@1.0.2 
  │   └── extsprintf@1.3.0 
  ├─┬ request@2.75.1-relativepath.1 
  │ ├── aws-sign2@0.6.0 
  │ ├── aws4@1.6.0 
  │ ├─┬ bl@1.1.2 
  │ │ └─┬ readable-stream@2.0.6 
  │ │   └── string_decoder@0.10.31 
  │ ├── caseless@0.11.0 
  │ ├── extend@3.0.1 
  │ ├── forever-agent@0.6.1 
  │ ├─┬ har-validator@2.0.6 
  │ │ ├─┬ is-my-json-valid@2.16.0 
  │ │ │ ├── generate-function@2.0.0 
  │ │ │ ├─┬ generate-object-property@1.2.0 
  │ │ │ │ └── is-property@1.0.2 
  │ │ │ └── jsonpointer@4.0.1 
  │ │ └─┬ pinkie-promise@2.0.1 
  │ │   └── pinkie@2.0.4 
  │ ├─┬ hawk@3.1.3 
  │ │ ├── boom@2.10.1 
  │ │ ├── cryptiles@2.0.5 
  │ │ ├── hoek@2.16.3 
  │ │ └── sntp@1.0.9 
  │ ├─┬ http-signature@1.1.1 
  │ │ ├── assert-plus@0.2.0 
  │ │ ├─┬ jsprim@1.4.1 
  │ │ │ └── json-schema@0.2.3 
  │ │ └─┬ sshpk@1.13.1 
  │ │   ├── bcrypt-pbkdf@1.0.1 
  │ │   ├── dashdash@1.14.1 
  │ │   ├── ecc-jsbn@0.1.1 
  │ │   ├── getpass@0.1.7 
  │ │   ├── jsbn@0.1.1 
  │ │   └── tweetnacl@0.14.5 
  │ ├── is-typedarray@1.0.0 
  │ ├── isstream@0.1.2 
  │ ├── json-stringify-safe@5.0.1 
  │ ├── node-uuid@1.4.8 
  │ ├── oauth-sign@0.8.2 
  │ ├── qs@6.2.3 
  │ ├── stringstream@0.0.5 
  │ ├─┬ tough-cookie@2.3.2 
  │ │ └── punycode@1.4.1 
  │ └── tunnel-agent@0.4.3 
  ├── semver@5.4.1 
  ├─┬ serialport@4.0.7 
  │ ├── bindings@1.2.1 
  │ ├── commander@2.11.0 
  │ ├─┬ debug@2.6.8 
  │ │ └── ms@2.0.0 
  │ ├─┬ lie@3.1.1 
  │ │ └── immediate@3.0.6 
  │ ├── nan@2.6.2 
  │ ├─┬ node-pre-gyp@0.6.32 
  │ │ ├─┬ mkdirp@0.5.1 
  │ │ │ └── minimist@0.0.8 
  │ │ ├─┬ nopt@3.0.6 
  │ │ │ └── abbrev@1.0.9 
  │ │ ├─┬ npmlog@4.0.1 
  │ │ │ ├─┬ are-we-there-yet@1.1.2 
  │ │ │ │ ├── delegates@1.0.0 
  │ │ │ │ └─┬ readable-stream@2.2.2 
  │ │ │ │   ├── buffer-shims@1.0.0 
  │ │ │ │   ├── core-util-is@1.0.2 
  │ │ │ │   ├── inherits@2.0.3 
  │ │ │ │   ├── isarray@1.0.0 
  │ │ │ │   ├── process-nextick-args@1.0.7 
  │ │ │ │   ├── string_decoder@0.10.31 
  │ │ │ │   └── util-deprecate@1.0.2 
  │ │ │ ├── console-control-strings@1.1.0 
  │ │ │ ├─┬ gauge@2.7.2 
  │ │ │ │ ├── aproba@1.0.4 
  │ │ │ │ ├── has-unicode@2.0.1 
  │ │ │ │ ├── object-assign@4.1.0 
  │ │ │ │ ├── signal-exit@3.0.2 
  │ │ │ │ ├─┬ string-width@1.0.2 
  │ │ │ │ │ ├── code-point-at@1.1.0 
  │ │ │ │ │ └─┬ is-fullwidth-code-point@1.0.0 
  │ │ │ │ │   └── number-is-nan@1.0.1 
  │ │ │ │ ├─┬ strip-ansi@3.0.1 
  │ │ │ │ │ └── ansi-regex@2.0.0 
  │ │ │ │ ├── supports-color@0.2.0 
  │ │ │ │ └── wide-align@1.1.0 
  │ │ │ └── set-blocking@2.0.0 
  │ │ ├─┬ rc@1.1.6 
  │ │ │ ├── deep-extend@0.4.1 
  │ │ │ ├── ini@1.3.4 
  │ │ │ ├── minimist@1.2.0 
  │ │ │ └── strip-json-comments@1.0.4 
  │ │ ├─┬ request@2.79.0 
  │ │ │ ├── aws-sign2@0.6.0 
  │ │ │ ├── aws4@1.5.0 
  │ │ │ ├── caseless@0.11.0 
  │ │ │ ├─┬ combined-stream@1.0.5 
  │ │ │ │ └── delayed-stream@1.0.0 
  │ │ │ ├── extend@3.0.0 
  │ │ │ ├── forever-agent@0.6.1 
  │ │ │ ├─┬ form-data@2.1.2 
  │ │ │ │ └── asynckit@0.4.0 
  │ │ │ ├─┬ har-validator@2.0.6 
  │ │ │ │ ├─┬ chalk@1.1.3 
  │ │ │ │ │ ├── ansi-styles@2.2.1 
  │ │ │ │ │ ├── escape-string-regexp@1.0.5 
  │ │ │ │ │ ├─┬ has-ansi@2.0.0 
  │ │ │ │ │ │ └── ansi-regex@2.0.0 
  │ │ │ │ │ ├─┬ strip-ansi@3.0.1 
  │ │ │ │ │ │ └── ansi-regex@2.0.0 
  │ │ │ │ │ └── supports-color@2.0.0 
  │ │ │ │ ├─┬ is-my-json-valid@2.15.0 
  │ │ │ │ │ ├── generate-function@2.0.0 
  │ │ │ │ │ ├─┬ generate-object-property@1.2.0 
  │ │ │ │ │ │ └── is-property@1.0.2 
  │ │ │ │ │ ├── jsonpointer@4.0.0 
  │ │ │ │ │ └── xtend@4.0.1 
  │ │ │ │ └─┬ pinkie-promise@2.0.1 
  │ │ │ │   └── pinkie@2.0.4 
  │ │ │ ├─┬ hawk@3.1.3 
  │ │ │ │ ├── boom@2.10.1 
  │ │ │ │ ├── cryptiles@2.0.5 
  │ │ │ │ ├── hoek@2.16.3 
  │ │ │ │ └── sntp@1.0.9 
  │ │ │ ├─┬ http-signature@1.1.1 
  │ │ │ │ ├── assert-plus@0.2.0 
  │ │ │ │ ├─┬ jsprim@1.3.1 
  │ │ │ │ │ ├── extsprintf@1.0.2 
  │ │ │ │ │ ├── json-schema@0.2.3 
  │ │ │ │ │ └── verror@1.3.6 
  │ │ │ │ └─┬ sshpk@1.10.1 
  │ │ │ │   ├── asn1@0.2.3 
  │ │ │ │   ├── assert-plus@1.0.0 
  │ │ │ │   ├── bcrypt-pbkdf@1.0.0 
  │ │ │ │   ├── dashdash@1.14.1 
  │ │ │ │   ├── ecc-jsbn@0.1.1 
  │ │ │ │   ├── getpass@0.1.6 
  │ │ │ │   ├── jodid25519@1.0.2 
  │ │ │ │   ├── jsbn@0.1.0 
  │ │ │ │   └── tweetnacl@0.14.4 
  │ │ │ ├── is-typedarray@1.0.0 
  │ │ │ ├── isstream@0.1.2 
  │ │ │ ├── json-stringify-safe@5.0.1 
  │ │ │ ├─┬ mime-types@2.1.13 
  │ │ │ │ └── mime-db@1.25.0 
  │ │ │ ├── oauth-sign@0.8.2 
  │ │ │ ├── qs@6.3.0 
  │ │ │ ├── stringstream@0.0.5 
  │ │ │ ├─┬ tough-cookie@2.3.2 
  │ │ │ │ └── punycode@1.4.1 
  │ │ │ ├── tunnel-agent@0.4.3 
  │ │ │ └── uuid@3.0.1 
  │ │ ├─┬ rimraf@2.5.4 
  │ │ │ └─┬ glob@7.1.1 
  │ │ │   ├── fs.realpath@1.0.0 
  │ │ │   ├─┬ inflight@1.0.6 
  │ │ │   │ └── wrappy@1.0.2 
  │ │ │   ├── inherits@2.0.3 
  │ │ │   ├─┬ minimatch@3.0.3 
  │ │ │   │ └─┬ brace-expansion@1.1.6 
  │ │ │   │   ├── balanced-match@0.4.2 
  │ │ │   │   └── concat-map@0.0.1 
  │ │ │   ├─┬ once@1.4.0 
  │ │ │   │ └── wrappy@1.0.2 
  │ │ │   └── path-is-absolute@1.0.1 
  │ │ ├── semver@5.3.0 
  │ │ ├─┬ tar@2.2.1 
  │ │ │ ├── block-stream@0.0.9 
  │ │ │ ├─┬ fstream@1.0.10 
  │ │ │ │ └── graceful-fs@4.1.11 
  │ │ │ └── inherits@2.0.3 
  │ │ └─┬ tar-pack@3.3.0 
  │ │   ├─┬ debug@2.2.0 
  │ │   │ └── ms@0.7.1 
  │ │   ├─┬ fstream@1.0.10 
  │ │   │ ├── graceful-fs@4.1.11 
  │ │   │ └── inherits@2.0.3 
  │ │   ├─┬ fstream-ignore@1.0.5 
  │ │   │ ├── inherits@2.0.3 
  │ │   │ └─┬ minimatch@3.0.3 
  │ │   │   └─┬ brace-expansion@1.1.6 
  │ │   │     ├── balanced-match@0.4.2 
  │ │   │     └── concat-map@0.0.1 
  │ │   ├─┬ once@1.3.3 
  │ │   │ └── wrappy@1.0.2 
  │ │   ├─┬ readable-stream@2.1.5 
  │ │   │ ├── buffer-shims@1.0.0 
  │ │   │ ├── core-util-is@1.0.2 
  │ │   │ ├── inherits@2.0.3 
  │ │   │ ├── isarray@1.0.0 
  │ │   │ ├── process-nextick-args@1.0.7 
  │ │   │ ├── string_decoder@0.10.31 
  │ │   │ └── util-deprecate@1.0.2 
  │ │   └── uid-number@0.0.6 
  │ └─┬ object.assign@4.0.4 
  │   ├─┬ define-properties@1.1.2 
  │   │ └── foreach@2.0.5 
  │   ├── function-bind@1.1.0 
  │   └── object-keys@1.0.11 
  ├─┬ softap-setup@4.1.0 
  │ └─┬ node-rsa@0.4.2 
  │   └── asn1@0.2.3 
  ├─┬ temp@0.8.3 
  │ ├── os-tmpdir@1.0.2 
  │ └── rimraf@2.2.8 
  ├── when@3.7.8 
  ├── xtend@4.0.1 
  ├─┬ yargs@5.0.0 
  │ ├─┬ cliui@3.2.0 
  │ │ └─┬ wrap-ansi@2.1.0 
  │ │   └─┬ string-width@1.0.2 
  │ │     └── is-fullwidth-code-point@1.0.0 
  │ ├── decamelize@1.2.0 
  │ ├── get-caller-file@1.0.2 
  │ ├── lodash.assign@4.2.0 
  │ ├─┬ read-pkg-up@1.0.1 
  │ │ ├─┬ find-up@1.1.2 
  │ │ │ └── path-exists@2.1.0 
  │ │ └─┬ read-pkg@1.1.0 
  │ │   ├── load-json-file@1.1.0 
  │ │   ├─┬ normalize-package-data@2.4.0 
  │ │   │ ├── hosted-git-info@2.5.0 
  │ │   │ ├─┬ is-builtin-module@1.0.0 
  │ │   │ │ └── builtin-modules@1.1.1 
  │ │   │ └─┬ validate-npm-package-license@3.0.1 
  │ │   │   ├─┬ spdx-correct@1.0.2 
  │ │   │   │ └── spdx-license-ids@1.2.2 
  │ │   │   └── spdx-expression-parse@1.0.4 
  │ │   └── path-type@1.1.0 
  │ ├── require-directory@2.1.1 
  │ ├── require-main-filename@1.0.1 
  │ ├── set-blocking@2.0.0 
  │ ├─┬ string-width@1.0.2 
  │ │ ├── code-point-at@1.1.0 
  │ │ └─┬ is-fullwidth-code-point@1.0.0 
  │ │   └── number-is-nan@1.0.1 
  │ ├── which-module@1.0.0 
  │ ├── window-size@0.2.0 
  │ ├── y18n@3.2.1 
  │ └─┬ yargs-parser@3.2.0 
  │   └── camelcase@3.0.0 
  ├─┬ yeoman-environment@1.6.6 
  │ ├── diff@2.2.3 
  │ ├─┬ globby@4.1.0 
  │ │ ├─┬ array-union@1.0.2 
  │ │ │ └── array-uniq@1.0.3 
  │ │ ├── arrify@1.0.1 
  │ │ ├── glob@6.0.4 
  │ │ ├── object-assign@4.1.1 
  │ │ └── pify@2.3.0 
  │ ├─┬ grouped-queue@0.3.3 
  │ │ └── lodash@4.17.4 
  │ ├─┬ inquirer@1.2.3 
  │ │ ├── ansi-escapes@1.4.0 
  │ │ ├─┬ cli-cursor@1.0.2 
  │ │ │ └─┬ restore-cursor@1.0.1 
  │ │ │   ├── exit-hook@1.1.1 
  │ │ │   └── onetime@1.1.0 
  │ │ ├─┬ external-editor@1.1.1 
  │ │ │ ├─┬ spawn-sync@1.0.15 
  │ │ │ │ ├─┬ concat-stream@1.6.0 
  │ │ │ │ │ └── typedarray@0.0.6 
  │ │ │ │ └── os-shim@0.1.3 
  │ │ │ └── tmp@0.0.29 
  │ │ ├── figures@1.7.0 
  │ │ ├── mute-stream@0.0.6 
  │ │ ├── rx@4.1.0 
  │ │ └─┬ string-width@1.0.2 
  │ │   └── is-fullwidth-code-point@1.0.0 
  │ ├── lodash@4.17.4 
  │ ├── log-symbols@1.0.2 
  │ ├─┬ mem-fs@1.1.3 
  │ │ ├─┬ vinyl@1.2.0 
  │ │ │ ├── clone@1.0.2 
  │ │ │ ├── clone-stats@0.0.1 
  │ │ │ └── replace-ext@0.0.1 
  │ │ └─┬ vinyl-file@2.0.0 
  │ │   ├─┬ strip-bom@2.0.0 
  │ │   │ └── is-utf8@0.2.1 
  │ │   └─┬ strip-bom-stream@2.0.0 
  │ │     └── first-chunk-stream@2.0.0 
  │ ├── text-table@0.2.0 
  │ └─┬ untildify@2.1.0 
  │   └── os-homedir@1.0.2 
  └─┬ yeoman-generator@1.1.1 
    ├── async@2.5.0 
    ├─┬ class-extend@0.1.2 
    │ └── object-assign@2.1.1 
    ├─┬ cross-spawn@5.1.0 
    │ ├─┬ lru-cache@4.1.1 
    │ │ ├── pseudomap@1.0.2 
    │ │ └── yallist@2.1.2 
    │ ├─┬ shebang-command@1.2.0 
    │ │ └── shebang-regex@1.0.0 
    │ └─┬ which@1.3.0 
    │   └── isexe@2.0.0 
    ├── dargs@5.1.0 
    ├── dateformat@2.0.0 
    ├── detect-conflict@1.0.1 
    ├─┬ error@7.0.2 
    │ └── string-template@0.2.1 
    ├─┬ find-up@2.1.0 
    │ └─┬ locate-path@2.0.0 
    │   └─┬ p-locate@2.0.0 
    │     └── p-limit@1.1.0 
    ├─┬ github-username@3.0.0 
    │ └─┬ gh-got@5.0.0 
    │   ├─┬ got@6.7.1 
    │   │ ├── duplexer3@0.1.4 
    │   │ ├── get-stream@3.0.0 
    │   │ ├── timed-out@4.0.1 
    │   │ └── unzip-response@2.0.1 
    │   └── is-plain-obj@1.1.0 
    ├─┬ glob@7.1.2 
    │ └── fs.realpath@1.0.0 
    ├─┬ istextorbinary@2.1.0 
    │ ├── binaryextensions@2.0.0 
    │ ├── editions@1.3.3 
    │ └── textextensions@2.1.0 
    ├── lodash@4.17.4 
    ├─┬ mem-fs-editor@3.0.2 
    │ ├── commondir@1.0.1 
    │ ├── deep-extend@0.4.2 
    │ ├── ejs@2.5.7 
    │ ├── glob@7.1.2 
    │ ├── globby@6.1.0 
    │ ├─┬ multimatch@2.1.0 
    │ │ └── array-differ@1.0.0 
    │ └─┬ vinyl@2.1.0 
    │   ├── clone@2.1.1 
    │   ├── clone-buffer@1.0.0 
    │   ├── clone-stats@1.0.0 
    │   ├── cloneable-readable@1.0.0 
    │   ├── remove-trailing-separator@1.0.2 
    │   └── replace-ext@1.0.0 
    ├── minimist@1.2.0 
    ├── path-exists@3.0.0 
    ├── pretty-bytes@4.0.2 
    ├─┬ read-chunk@2.1.0 
    │ ├── pify@3.0.0 
    │ └── safe-buffer@5.1.1 
    ├─┬ read-pkg-up@2.0.0 
    │ └─┬ read-pkg@2.0.0 
    │   ├─┬ load-json-file@2.0.0 
    │   │ └── strip-bom@3.0.0 
    │   └── path-type@2.0.0 
    ├─┬ rimraf@2.6.1 
    │ └── glob@7.1.2 
    ├─┬ shelljs@0.7.8 
    │ ├── glob@7.1.2 
    │ ├── interpret@1.0.3 
    │ └─┬ rechoir@0.6.2 
    │   └─┬ resolve@1.4.0 
    │     └── path-parse@1.0.5 
    ├── through2@2.0.3 
    └── user-home@2.0.0 
```

# Install git (if you have not already)

```
apt-get install -y git
```

# Setup a project folder

```
mkdir /spark
chown pi /spark
chgrp pi /spark
```


# Install spark server software

If you are root, you may need to become the `pi` user to do the 
remaining steps.

```
su - pi
```

As the `pi` user:

```
cd /spark
git clone https://github.com/spark/spark-server.git
cd spark-server
npm install
node main.js
```

Result:

```
> ursa@0.9.4 install /spark/spark-server/node_modules/ursa
> node-gyp rebuild

make: Entering directory '/spark/spark-server/node_modules/ursa/build'
  CXX(target) Release/obj.target/ursaNative/src/ursaNative.o
    SOLINK_MODULE(target) Release/obj.target/ursaNative.node
      COPY Release/ursaNative.node
      make: Leaving directory '/spark/spark-server/node_modules/ursa/build'
      spark-server@0.1.1 /spark/spark-server
      ├─┬ express@3.4.8 
      │ ├── buffer-crc32@0.2.1 
      │ ├─┬ commander@1.3.2 
      │ │ └── keypress@0.1.0 
      │ ├─┬ connect@2.12.0 
      │ │ ├── batch@0.5.0 
      │ │ ├── bytes@0.2.1 
      │ │ ├─┬ multiparty@2.2.0 
      │ │ │ ├─┬ readable-stream@1.1.14 
      │ │ │ │ ├── core-util-is@1.0.2 
      │ │ │ │ ├── inherits@2.0.3 
      │ │ │ │ ├── isarray@0.0.1 
      │ │ │ │ └── string_decoder@0.10.31 
      │ │ │ └── stream-counter@0.2.0 
      │ │ ├── negotiator@0.3.0 
      │ │ ├── pause@0.0.1 
      │ │ ├── qs@0.6.6 
      │ │ ├── raw-body@1.1.2 
      │ │ └── uid2@0.0.3 
      │ ├── cookie@0.1.0 
      │ ├── cookie-signature@1.0.1 
      │ ├── debug@0.8.1 
      │ ├── fresh@0.2.0 
      │ ├── merge-descriptors@0.0.1 
      │ ├── methods@0.1.0 
      │ ├── mkdirp@0.3.5 
      │ ├── range-parser@0.0.4 
      │ └─┬ send@0.1.4 
      │   └── mime@1.2.11 
      ├─┬ hogan-express@0.5.2 
      │ └─┬ hogan.js@3.0.2 
      │   ├── mkdirp@0.3.0 
      │   └─┬ nopt@1.0.10 
      │     └── abbrev@1.1.0 
      ├── moment@2.18.1 
      ├── node-oauth2-server@1.5.3 
      ├─┬ request@2.81.0 
      │ ├── aws-sign2@0.6.0 
      │ ├── aws4@1.6.0 
      │ ├── caseless@0.12.0 
      │ ├─┬ combined-stream@1.0.5 
      │ │ └── delayed-stream@1.0.0 
      │ ├── extend@3.0.1 
      │ ├── forever-agent@0.6.1 
      │ ├─┬ form-data@2.1.4 
      │ │ └── asynckit@0.4.0 
      │ ├─┬ har-validator@4.2.1 
      │ │ ├─┬ ajv@4.11.8 
      │ │ │ ├── co@4.6.0 
      │ │ │ └─┬ json-stable-stringify@1.0.1 
      │ │ │   └── jsonify@0.0.0 
      │ │ └── har-schema@1.0.5 
      │ ├─┬ hawk@3.1.3 
      │ │ ├── boom@2.10.1 
      │ │ ├── cryptiles@2.0.5 
      │ │ ├── hoek@2.16.3 
      │ │ └── sntp@1.0.9 
      │ ├─┬ http-signature@1.1.1 
      │ │ ├── assert-plus@0.2.0 
      │ │ ├─┬ jsprim@1.4.1 
      │ │ │ ├── assert-plus@1.0.0 
      │ │ │ ├── extsprintf@1.3.0 
      │ │ │ ├── json-schema@0.2.3 
      │ │ │ └─┬ verror@1.10.0 
      │ │ │   └── assert-plus@1.0.0 
      │ │ └─┬ sshpk@1.13.1 
      │ │   ├── asn1@0.2.3 
      │ │   ├── assert-plus@1.0.0 
      │ │   ├── bcrypt-pbkdf@1.0.1 
      │ │   ├─┬ dashdash@1.14.1 
      │ │   │ └── assert-plus@1.0.0 
      │ │   ├── ecc-jsbn@0.1.1 
      │ │   ├─┬ getpass@0.1.7 
      │ │   │ └── assert-plus@1.0.0 
      │ │   ├── jsbn@0.1.1 
      │ │   └── tweetnacl@0.14.5 
      │ ├── is-typedarray@1.0.0 
      │ ├── isstream@0.1.2 
      │ ├── json-stringify-safe@5.0.1 
      │ ├─┬ mime-types@2.1.16 
      │ │ └── mime-db@1.29.0 
      │ ├── oauth-sign@0.8.2 
      │ ├── performance-now@0.2.0 
      │ ├── qs@6.4.0 
      │ ├── safe-buffer@5.1.1 
      │ ├── stringstream@0.0.5 
      │ ├─┬ tough-cookie@2.3.2 
      │ │ └── punycode@1.4.1 
      │ ├── tunnel-agent@0.6.0 
      │ └── uuid@3.1.0 
      ├─┬ spark-protocol@0.1.6 
      │ ├── buffer-crc32@0.2.13 
      │ ├── h5.buffers@0.1.1 
      │ └── h5.coap@0.0.0  (git+https://github.com/morkai/h5.coap.git#51c3b2a4cb1af7f43d20e25a9d8658fed9169b9c)
      ├─┬ ursa@0.9.4 
      │ ├── bindings@1.3.0 
      │ └── nan@2.6.2 
      ├── when@3.7.8 
      └── xtend@4.0.1 
```

# Start the server

Result:

```
-------
No users exist, you should create some users!
-------
connect.multipart() will be removed in connect 3.0
visit https://github.com/senchalabs/connect/wiki/Connect-3.0 for alternatives
connect.limit() will be removed in connect 3.0
Starting server, listening on 8080
static class init!
core keys directory did't exist, creating... /spark/spark-server/core_keys
Creating NEW server key
Loading server key from default_key.pem
set server key
server public key is:  -----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA2yEqNYbxnL9gdpQXKi+W
i5hcqs/NOKUKu142Tx/thEhKMUoEeZDT6/pdZn/CoNs45Xd0BFy1fmB57ca8iofx
ajCMNQUTY8BPxHYvSpNHCFDnu9aVhi7E4JYt5nn+Z4Kig1pTFu+nBHFLe9yYc28y
asLtABtmoGN6ru/V/2kQqhChmzJ/wFmju27FPHg9L57yTGM5UwU0oAulcDeUX59J
+mmzdcuOtnPkxq6VVIU3rnBzYOFKZ2akHfypbxGwrioxZx9NF1215vHLHwCYAOIS
K/qRPWfnJDfsfSnYJrI9h7QS+WUZxe5CYslpMdJ1yFl5Sgpk45HDW/ywDBK5UgBx
IwIDAQAB
-----END PUBLIC KEY-----

Your server IP address is: 10.0.1.29
Your server IP address is: 10.1.0.1
server started { host: 'localhost', port: 5683 }
```

NOTE: I have two server addresses!  The 10.0.1.29 is my real local LAN.
The actual local Cloud we setup begins at 10.1.0.1.

# Create a local cloud profile in a working Particle CLI toolchain

Create a "localCloud" profile in the toolchain.  NOTE: The name
"localCloud" can be anything that YOU want.  You will need to refer
to whatever name you choose in the future.

```
particle config localCloud apiUrl "http://10.1.0.1:8080"
```

We can switch back and forth now between the real particle cloud
and our local cloud.   You can see all the profiles using
`particle config list`.

## Particle cloud

```
particle config particle
```

## The local cloud

```
particle config localCloud
```

# Start setting up the "localCloud" server

NOTE: These commands are run on the server running the "localCloud"
software.  One terminal is in use now running `node main.js`, log in
again and get to the command line to run more commands to set
things up.

## Log in again and switch to the `pi` user

Create a "localCloud" configuration on the Particle CLI toolchain
on the server.

```
cd /spark/spark-server
particle config localCloud apiUrl "http://10.1.0.1:8080"
particle config localCloud
particle setup
```

Break out of the setup sequence when asked to send Wifi-credtentials, etc...

```
> Great success! You're now the owner of a brand new account!
```

Initial screen: proceed with "Create a new account".

```
 _ __             _   _      _        
 | '_ \  __ _ _ __| |_(_) ___| | ___ 
 | |_) |/ _` | '__| __| |/ __| |/ _ \
 |  __/| (_| | |  | |_| | (__| |  __/
 |_|    \__,_|_|   \__|_|\___|_|\___|
                  https://particle.io

> Setup is easy! Let's get started...
? Hello Stranger! This seems to be your first time here. What would you like to do? (Use arrow keys)
❯ Create a new account 
Login 
```

# Configure a Photon

Plug in a Photon and place into DFU mode [flashing yellow].

# Change server keys to local cloud key + IP address

```
$ particle keys server default_key.pub.pem 10.1.0.1
Found DFU device 2b04:d006
Creating DER format file
spawning dfu-util -d 2b04:d006 -a 1 -i 0 -s 2082 -D default_key.pub-10_1_0_1-rsa.der
No valid DFU suffix signature
Warning: File has no DFU suffix
dfu-util 0.7

Copyright 2005-2008 Weston Schmidt, Harald Welte and OpenMoko Inc.
Copyright 2010-2012 Tormod Volden and Stefan Schmidt
This program is Free Software and has ABSOLUTELY NO WARRANTY
Please report bugs to dfu-util@lists.gnumonks.org

Filter on vendor = 0x2b04 product = 0xd006
Opening DFU capable USB device... ID 2b04:d006
Run-time device DFU version 011a
Found DFU: [2b04:d006] devnum=0, cfg=1, intf=0, alt=1, name="@DCT Flash   /0x00000000/01*016Kg"
Claiming USB DFU Interface...
Setting Alternate Setting #1 ...
Determining device status: state = dfuIDLE, status = 0
dfuIDLE, continuing
DFU mode device DFU version 011a
Device returned transfer size 4096
DfuSe interface name: "DCT Flash   "
Downloading to address = 0x00000822, size = 512
.
File downloaded successfully
Okay!  New keys in place, your device will not restart.
```

# Get your Photon device ID

Put the Photon into serial listening mode. [flashing blue]

`particle serial identify`

# Place the Particle device public key in the core_keys directory

Put the Photon into DFU mode.

```
cd core_keys
particle keys save INPUT_DEVICE_ID_HERE
```

Reset the Photon by pressing the reset (RST) button.

# Claim the Photon

NOTE: You should claim the Photon in the normal Particle Cloud before
attaching to the localCloud.

```
particle setup
```

# Check for a connection


