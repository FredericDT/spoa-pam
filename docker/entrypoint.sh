#!/bin/bash

echo "${FDT_SPOA_PAM_RADIUS_HOST}   ${FDT_SPOA_PAM_RADIUS_SHARED_SECRET}    ${FDT_SPOA_PAM_RADIUS_TIMEOUT}" > /etc/pam_radius_auth.conf

ping -c 4 "${FDT_SPOA_PAM_RADIUS_HOST}" # I donot know why exactly I need this, perhaps arp?

/usr/sbin/spoa "${@}"