#!/usr/bin/python
import sys
import os
import json
import csv
import subprocess

from collections import OrderedDict


# Replace argument by data
def replace_in_list(self, my_list, search_term, replacement):
	temp_list = my_list.copy()
	for i in range(len(params)):
		if temp_list[i] == search_term:
			temp_list[i] = replacement

	return temp_list


def get_list_from_json_array(json_obj, json_tag):
	my_list = []
	for item in json_obj[json_tag]:
		my_list.append(item)

	return my_list


def get_list_from_json(json_obj, json_tag):
	if json_tag in json_obj:
		return list(sum(json_obj[json_tag].items(), ()))
	else:
		return None




# Utils
def validate_num_args(num_arguments, min_count):
	if num_arguments < min_count:
		print ("At least " + str(min_count) + " arguments are needed!")
		exit(1)

	return True


def splitString(filename, char):
	k = filename.rfind(char)
	return filename[:k+1], filename[k+1:]



# Commands
def run_command(cmd):
	return subprocess.call(cmd, shell=True)


def get_git_version(path):
	git_tag = os.popen("cd " + path + "; git describe --tags").read()
	return git_tag


# Folders
def create_folder(path):
	if not os.path.exists(path):
		try:  
			os.makedirs(path)
		except OSError:  
			print ("Creation of the directory %s failed" % path)
		else:  
			print ("Successfully created the directory %s " % path)


def list_files_in_folder(folder):
	files = []

	# r=root, d=directories, f = files
	for r, d, f in os.walk(folder):
		for file in f:
			files.append(file)

	return files


def list_files_in_folder(folder, extension):
	files = []

	# r=root, d=directories, f = files
	for r, d, f in os.walk(folder):
		for file in f:
			if file.endswith(extension):
				files.append( folder + "/" + file)

	return files




# CSV
def open_csv_file(csv_file):
	with open(csv_file,'r') as csvinput:
		reader = csv.reader(csvinput)
		return reader


def write_csv(csv_file, contents):
	with open(csv_file, 'w') as csvoutput:
		writer = csv.writer(csvoutput, lineterminator='\n')
		writer.writerows(contents)


def extract_csv_col(filename, delimiter_char, colpos):
	if ( not os.path.isfile(filename) ):
		print( filename + " does not exist! ")
		return None

	with open(filename) as csv_file:
		try:
			csv_data = csv.reader(csv_file, delimiter=delimiter_char)
			col = []
			for row in csv_data:
				col.append( float(row[colpos]) )

			return col

		except ValueError as e:
			print ("CSV file " + filename + " is invalid! " + str(e))
			exit(1)


def get_csv_header(filename):
	with open(csv_file,'r') as csvinput:
		reader = csv.reader(csvinput)
		header = next(reader)
		return header


def append_list_as_row(filename, list_of_elem):
	# Open file in append mode
	with open(filename, 'a+', newline='') as write_obj:
		# Create a writer object from csv module
		csv_writer = writer(write_obj)
		# Add contents of list as last row in the csv file
		csv_writer.writerow(list_of_elem)


# JSON
def write_json_file(json_file_name, contents):
	with open(json_file_name, 'w') as file_out:
		file_out.write( json.dumps(contents, indent=4) )
		file_out.close()


def read_json(filename):
	# Check if file exists
	if ( not os.path.isfile(filename) ):
		print( filename + " does not exist! ")
		return None
		
	# Open json file
	with open(filename, "r") as read_file:
		try:
			json_data = json.load(read_file,  object_pairs_hook=OrderedDict)
			return json_data
		except ValueError as e:
			print ("JSON file " + filename + " is invalid! \n\t" + str(e) + "\n")
			return None





	