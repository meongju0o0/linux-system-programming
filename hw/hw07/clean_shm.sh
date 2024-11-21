#!/bin/bash

OWNER="lsp30"

ipcs -m | grep "$OWNER" | awk '{print $2}' | while read shmid; do
	echo "Deleting shared memory ID: $shmid (owner: $OWNER)"
	ipcrm -m $shmid
done
