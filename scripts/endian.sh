#!/bin/bash

echo -n I | hexdump -o | awk '{ print substr($2, 6, 1); exit }'
