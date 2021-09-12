#!/usr/bin/env bash

set -o pipefail
set -o errexit
set -o xtrace

PROJECT_ROOT=$(git rev-parse --show-toplevel)
cd "$PROJECT_ROOT"

ssh bryanag2@fa21-cs425-g60-02.cs.illinois.edu 'sleep 10 && python3 -u /home/bryanag2/cs-425/mp0/scripts/generator.py 5 700 | /home/bryanag2/cs-425/mp0/bin/node A 172.22.156.179 8082' &
ssh bryanag2@fa21-cs425-g60-03.cs.illinois.edu 'sleep 10 && python3 -u /home/bryanag2/cs-425/mp0/scripts/generator.py 5 700 | /home/bryanag2/cs-425/mp0/bin/node B 172.22.156.179 8082' &
ssh bryanag2@fa21-cs425-g60-04.cs.illinois.edu 'sleep 10 && python3 -u /home/bryanag2/cs-425/mp0/scripts/generator.py 5 700 | /home/bryanag2/cs-425/mp0/bin/node C 172.22.156.179 8082' &
ssh bryanag2@fa21-cs425-g60-05.cs.illinois.edu 'sleep 10 && python3 -u /home/bryanag2/cs-425/mp0/scripts/generator.py 5 700 | /home/bryanag2/cs-425/mp0/bin/node D 172.22.156.179 8082' &
ssh bryanag2@fa21-cs425-g60-06.cs.illinois.edu 'sleep 10 && python3 -u /home/bryanag2/cs-425/mp0/scripts/generator.py 5 700 | /home/bryanag2/cs-425/mp0/bin/node E 172.22.156.179 8082' &
ssh bryanag2@fa21-cs425-g60-07.cs.illinois.edu 'sleep 10 && python3 -u /home/bryanag2/cs-425/mp0/scripts/generator.py 5 700 | /home/bryanag2/cs-425/mp0/bin/node F 172.22.156.179 8082' &
ssh bryanag2@fa21-cs425-g60-08.cs.illinois.edu 'sleep 10 && python3 -u /home/bryanag2/cs-425/mp0/scripts/generator.py 5 700 | /home/bryanag2/cs-425/mp0/bin/node G 172.22.156.179 8082' &
ssh bryanag2@fa21-cs425-g60-09.cs.illinois.edu 'sleep 10 && python3 -u /home/bryanag2/cs-425/mp0/scripts/generator.py 5 700 | /home/bryanag2/cs-425/mp0/bin/node H 172.22.156.179 8082' &

ssh -t bryanag2@fa21-cs425-g60-01.cs.illinois.edu '/home/bryanag2/cs-425/mp0/bin/logger 8082'
