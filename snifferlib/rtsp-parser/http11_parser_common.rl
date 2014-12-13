%%{

  machine http_parser_common;

#### HTTP PROTOCOL GRAMMAR
# line endings

StatusCode  = (("100"     
                |     "200"    
                |     "201"
                |     "250"
                |     "300"
                |     "301"
                |     "302"
                |     "303"
                |     "304"
                |     "305"
                |     "400"
                |     "401"
                |     "402"
                |     "403"
                |     "404"
                |     "405"
                |     "406"
                |     "407"
                |     "408"
                |     "410"
                |     "411"
                |     "412"
                |     "413"
                |     "414"
                |     "415"
                |     "451"
                |     "452"
                |     "453"
                |     "454"
                |     "455"
                |     "456"
                |     "457"
                |     "458"
                |     "459"
                |     "460"
                |     "461"
                |     "462"
                |     "500"
                |     "501"
                |     "502"
                |     "503"
                |     "504"
                |     "505"
                |     "551") >start_response_state  %end_response_state);
                
                
RTSP_Method_DESCRIBE		= "DESCRIBE" %ragle_protocal_rtsp_Method_DESCRIBE;
RTSP_Method_ANNOUNCE		= "ANNOUNCE" %ragle_protocal_rtsp_Method_ANNOUNCE;
RTSP_Method_GET_PARAMETER	= "GET_PARAMETER" %ragle_protocal_rtsp_Method_GET_PARAMETER;
RTSP_Method_OPTIONS			="OPTIONS" %ragle_protocal_rtsp_Method_OPTIONS;
RTSP_Method_PAUSE			="PAUSE" %ragle_protocal_rtsp_Method_PAUSE;
RTSP_Method_PLAY			="PLAY" %ragle_protocal_rtsp_Method_PLAY;
RTSP_Method_RECORD			="RECORD" %ragle_protocal_rtsp_Method_RECORD;
RTSP_Method_REDIRECT		="REDIRECT" %ragle_protocal_rtsp_Method_REDIRECT;
RTSP_Method_SETUP			="SETUP" %ragle_protocal_rtsp_Method_SETUP;
RTSP_Method_SET_PARAMETER	="SET_PARAMETER" %ragle_protocal_rtsp_Method_SET_PARAMETER;
RTSP_Method_TEARDOWN		="TEARDOWN" %ragle_protocal_rtsp_Method_TEARDOWN;

RTSP_Method=(	RTSP_Method_DESCRIBE
				| RTSP_Method_ANNOUNCE
				| RTSP_Method_GET_PARAMETER
				| RTSP_Method_OPTIONS
				| RTSP_Method_PAUSE
				| RTSP_Method_PLAY
				| RTSP_Method_RECORD
				| RTSP_Method_REDIRECT
				| RTSP_Method_SETUP
				| RTSP_Method_SET_PARAMETER
				| RTSP_Method_TEARDOWN
				);                          
CRLF = "\r\n";

# character types
CTL = (cntrl | 127);
safe = ("$" | "-" | "_" | ".");
extra = ("!" | "*" | "'" | "(" | ")" | ",");
reserved = (";" | "/" | "?" | ":" | "@" | "&" | "=" | "+");
sorta_safe = ("\"" | "<" | ">");
unsafe = (CTL | " " | "#" | "%" | sorta_safe);
national = any -- (alpha | digit | reserved | extra | safe | unsafe);
unreserved = (alpha | digit | safe | extra | national);
escape = ("%" xdigit xdigit);
uchar = (unreserved | escape | sorta_safe);
pchar = (uchar | ":" | "@" | "&" | "=" | "+");
tspecials = ("(" | ")" | "<" | ">" | "@" | "," | ";" | ":" | "\\" | "\"" | "/" | "[" | "]" | "?" | "=" | "{" | "}" | " " | "\t");

# elements
token = (ascii -- (CTL | tspecials));

# URI schemes and absolute paths
scheme = ( alpha | digit | "+" | "-" | "." )* ;
absolute_uri = (scheme ":" (uchar | reserved )*);

path = ( pchar+ ( "/" pchar* )* ) ;
query = ( uchar | reserved )* %query_string ;
param = ( pchar | "/" )* ;
params = ( param ( ";" param )* ) ;
rel_path = ( path? %request_path (";" params)? ) ("?" %start_query query)?;

#xxx
port           = digit+;
hostnumber     = digit+ "." digit+ "." digit+ "." digit+;
alphadigit     = alpha | digit;
toplabel       = alpha | alpha *[ alphadigit | "-" ] alphadigit;
domainlabel    = alphadigit | alphadigit [ alphadigit | "-" ]* alphadigit;
hostname       = [ domainlabel "." ]* toplabel;
host           = (hostname | hostnumber);
hostport       = (host [ ":" port ]);
hsegment       = ([ uchar | ";" | ":" | "@" | "&" | "=" ]?);
hpath          = (hsegment [ "/" hsegment ]?);
httpurl        =("http://" hostport [ "/" hpath ] [ "?" query ]);
rtspurl        =("rtsp://" hostport [ "/" hpath ] [ "?" query ]);

#xxx
absolute_path = ( "/"+ rel_path );

#Request_URI = ( "*" | absolute_uri | absolute_path ) >mark %request_uri;
Request_URI = ( "rtsp://" rel_path ) >mark %request_uri;
Fragment = ( uchar | reserved )* >mark %fragment;

#Method = ( upper | digit | safe ){1,20} >mark %request_method;

Method = RTSP_Method  >mark %request_method;

rtsp_number = ( digit+ "." digit+ ) ;
RTSP_Version = ( "RTSP/" rtsp_number ) >mark %http_version ;
Request_Line = ( Method " "* Request_URI ("#" Fragment){0,1} " "* RTSP_Version " "? CRLF ) ;

Respone_Line = ( RTSP_Version " "*  StatusCode " "* ( upper | digit | safe ){1,5} " "? CRLF %ragle_protocal_rtsp_Method_RESPONE %write_response_state) ;

field_name = ( token -- ":" )+ >start_field $snake_upcase_field %write_field;

field_value = any* >start_value %write_value;

message_header = field_name ":" " "* field_value :> CRLF;

Request = (Request_Line | Respone_Line )( message_header )* ( CRLF @done );

main := Request;

}%%
