#!/bin/bash

rsync --append --inplace --progress "$@"
