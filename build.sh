#!/bin/bash

DIR_DRIVE = "/cygdrive/c/"

# edit this for your own use...
PROJECT_PATH = "Users/nickk/OneDrive/Documents/GitHub/UnstableKernel"

CYG_PROJECT_PATH = "$DIR_DRIVE$PROJECT_PATH"

mkdir WORK_ENVIORMENT

cp -r $CYG_PROJECT_PATH/source WORK_ENVIORMENT