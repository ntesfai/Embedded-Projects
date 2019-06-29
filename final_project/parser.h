/**
    Author: Nubian Tesfai
    Instructor: Douglas Sandy
    Course: SER486, Arizona State University

    This class defines the functionality for parsing
    an HTTP request from the master controller to the AVR.
    Data is received from a socket and must be parsed in order
    to respond accordingly.
*/
#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

//This method parses the incoming HTTP request from the socket
//Returns the unique value for the request line
int parser_parse();

//Method for writing nbsp& in the HTTP response.
//Parameter is howmany times it is printed
void parser_print_nbsp(unsigned char times);

//Based on the results from parse() respond with a well formed HTTP response
void parser_response();

#endif
