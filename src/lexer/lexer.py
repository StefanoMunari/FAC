#!/usr/bin/python
############################################################
# The script generates the FAC-lexical analyzer as lexer.fl
############################################################
import os
import json
import shutil

ROOT_DIR=os.getcwd()+"/lexer/"
TARGET_CONF="conf"
OUTPUT_FILE="lexer.fl"

os.chdir(ROOT_DIR+TARGET_CONF)

with open(ROOT_DIR+OUTPUT_FILE, 'w+') as output_file:
    with open('header.h', 'rb') as header, open('regex.json', 'rb') as regexs, open('rules.json', 'rb') as rules:
        ## HEADER 
        output_file.write("%{\n")
        shutil.copyfileobj(header, output_file)
        output_file.write("\n%}")
        header.close()
        ## REGEX
        os.chdir(ROOT_DIR+"regex")
        regexs=json.load(regexs)
        # extract list of regexs
        regexs=regexs["regex"]
        for rx in regexs:
        	data=open(rx+".fl")
        	output_file.write("\n")
        	shutil.copyfileobj(data, output_file)
        	data.close()
        output_file.write("\n%%")
        ## TRANSLATION RULES
        os.chdir(ROOT_DIR+"rules")
        rules=json.load(rules)
        # extract list of rules
        rules=rules["rules"]
        for rule in rules:
        	data=open(rule+".fl")
        	output_file.write("\n")
        	shutil.copyfileobj(data, output_file)
        	data.close()
        output_file.write("\n%%\n")
