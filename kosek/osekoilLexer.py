# $ANTLR 3.4 osekoil.g 2013-05-16 18:39:25

import sys
from antlr3 import *
from antlr3.compat import set, frozenset



# for convenience in actions
HIDDEN = BaseRecognizer.HIDDEN

# token types
EOF=-1
T__76=76
T__77=77
T__78=78
T__79=79
T__80=80
T__81=81
T__82=82
T__83=83
T__84=84
T__85=85
T__86=86
T__87=87
T__88=88
T__89=89
T__90=90
T__91=91
T__92=92
T__93=93
T__94=94
T__95=95
T__96=96
T__97=97
T__98=98
T__99=99
T__100=100
T__101=101
T__102=102
T__103=103
T__104=104
T__105=105
T__106=106
T__107=107
T__108=108
T__109=109
T__110=110
T__111=111
T__112=112
T__113=113
T__114=114
T__115=115
T__116=116
T__117=117
T__118=118
T__119=119
T__120=120
APPLICATION_DEFINITION=4
ASSIGN=5
ATTRIBUTE_NAME=6
ATTRIBUTE_VALUE=7
AUTO_SPECIFIER=8
AUTO_VALUE=9
BOOLEAN=10
BOOL_VALUE=11
BOOL_VALUES=12
COMMENT=13
DECDIGITS=14
DEFAULT_BOOL=15
DEFAULT_FLOAT=16
DEFAULT_NAME=17
DEFAULT_NUMBER=18
DEFAULT_STRING=19
DESCRIPTION=20
ENUMERATION=21
ENUMERATOR=22
ENUMERATOR_LIST=23
ESC_SEQ=24
EXPONENT=25
FILE=26
FLOAT=27
FLOAT_DEF=28
FLOAT_RANGE=29
FLOAT_VALUE=30
HEX=31
HEX_DIGIT=32
ID=33
ID_VALUE=34
IMPLEMENTATION_DEF=35
IMPLEMENTATION_DEFINITION=36
IMPLEMENTATION_LIST=37
IMPLEMENTATION_SPEC=38
IMPLEMENTATION_SPEC_LIST=39
IMPL_ATTR_DEF=40
IMPL_DEF_LIST=41
IMPL_PARAMETER_LIST=42
IMPL_REF_DEF=43
INT=44
LBRACK=45
LCURLY=46
LINE_NUMBER=47
LPAREN=48
MULTIPLE_SPECIFIER=49
NUMBER=50
NUMBER_LIST=51
NUMBER_RANGE=52
NUMBER_VALUE=53
OBJECT=54
OBJECT_DEFINITION=55
OBJECT_DEFINITION_LIST=56
OBJECT_NAME=57
OBJECT_REF_TYPE=58
OCTAL_ESC=59
OIL_VERSION=60
PARAMETER=61
PARAMETER_LIST=62
POSDIGIT=63
RBRACK=64
RCURLY=65
REFERENCE_NAME=66
RRAREN=67
SEMI=68
SHIT=69
SIGN=70
STRING=71
STRING_VALUE=72
UNICODE_ESC=73
WS=74
ZERODIGIT=75


class osekoilLexer(Lexer):

    grammarFileName = "osekoil.g"
    api_version = 1

    def __init__(self, input=None, state=None):
        if state is None:
            state = RecognizerSharedState()
        super(osekoilLexer, self).__init__(input, state)

        self.delegates = []

        self.dfa9 = self.DFA9(
            self, 9,
            eot = self.DFA9_eot,
            eof = self.DFA9_eof,
            min = self.DFA9_min,
            max = self.DFA9_max,
            accept = self.DFA9_accept,
            special = self.DFA9_special,
            transition = self.DFA9_transition
            )

        self.dfa21 = self.DFA21(
            self, 21,
            eot = self.DFA21_eot,
            eof = self.DFA21_eof,
            min = self.DFA21_min,
            max = self.DFA21_max,
            accept = self.DFA21_accept,
            special = self.DFA21_special,
            transition = self.DFA21_transition
            )


                           
        pass





    # $ANTLR start "T__76"
    def mT__76(self, ):
        try:
            _type = T__76
            _channel = DEFAULT_CHANNEL

            # osekoil.g:11:7: ( ',' )
            # osekoil.g:11:9: ','
            pass 
            self.match(44)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__76"



    # $ANTLR start "T__77"
    def mT__77(self, ):
        try:
            _type = T__77
            _channel = DEFAULT_CHANNEL

            # osekoil.g:12:7: ( '..' )
            # osekoil.g:12:9: '..'
            pass 
            self.match("..")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__77"



    # $ANTLR start "T__78"
    def mT__78(self, ):
        try:
            _type = T__78
            _channel = DEFAULT_CHANNEL

            # osekoil.g:13:7: ( ':' )
            # osekoil.g:13:9: ':'
            pass 
            self.match(58)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__78"



    # $ANTLR start "T__79"
    def mT__79(self, ):
        try:
            _type = T__79
            _channel = DEFAULT_CHANNEL

            # osekoil.g:14:7: ( 'ALARM' )
            # osekoil.g:14:9: 'ALARM'
            pass 
            self.match("ALARM")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__79"



    # $ANTLR start "T__80"
    def mT__80(self, ):
        try:
            _type = T__80
            _channel = DEFAULT_CHANNEL

            # osekoil.g:15:7: ( 'ALARM_TYPE' )
            # osekoil.g:15:9: 'ALARM_TYPE'
            pass 
            self.match("ALARM_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__80"



    # $ANTLR start "T__81"
    def mT__81(self, ):
        try:
            _type = T__81
            _channel = DEFAULT_CHANNEL

            # osekoil.g:16:7: ( 'APPMODE' )
            # osekoil.g:16:9: 'APPMODE'
            pass 
            self.match("APPMODE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__81"



    # $ANTLR start "T__82"
    def mT__82(self, ):
        try:
            _type = T__82
            _channel = DEFAULT_CHANNEL

            # osekoil.g:17:7: ( 'APPMODE_TYPE' )
            # osekoil.g:17:9: 'APPMODE_TYPE'
            pass 
            self.match("APPMODE_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__82"



    # $ANTLR start "T__83"
    def mT__83(self, ):
        try:
            _type = T__83
            _channel = DEFAULT_CHANNEL

            # osekoil.g:18:7: ( 'AUTO' )
            # osekoil.g:18:9: 'AUTO'
            pass 
            self.match("AUTO")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__83"



    # $ANTLR start "T__84"
    def mT__84(self, ):
        try:
            _type = T__84
            _channel = DEFAULT_CHANNEL

            # osekoil.g:19:7: ( 'BOOLEAN' )
            # osekoil.g:19:9: 'BOOLEAN'
            pass 
            self.match("BOOLEAN")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__84"



    # $ANTLR start "T__85"
    def mT__85(self, ):
        try:
            _type = T__85
            _channel = DEFAULT_CHANNEL

            # osekoil.g:20:7: ( 'COM' )
            # osekoil.g:20:9: 'COM'
            pass 
            self.match("COM")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__85"



    # $ANTLR start "T__86"
    def mT__86(self, ):
        try:
            _type = T__86
            _channel = DEFAULT_CHANNEL

            # osekoil.g:21:7: ( 'COM_TYPE' )
            # osekoil.g:21:9: 'COM_TYPE'
            pass 
            self.match("COM_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__86"



    # $ANTLR start "T__87"
    def mT__87(self, ):
        try:
            _type = T__87
            _channel = DEFAULT_CHANNEL

            # osekoil.g:22:7: ( 'COUNTER' )
            # osekoil.g:22:9: 'COUNTER'
            pass 
            self.match("COUNTER")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__87"



    # $ANTLR start "T__88"
    def mT__88(self, ):
        try:
            _type = T__88
            _channel = DEFAULT_CHANNEL

            # osekoil.g:23:7: ( 'COUNTER_TYPE' )
            # osekoil.g:23:9: 'COUNTER_TYPE'
            pass 
            self.match("COUNTER_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__88"



    # $ANTLR start "T__89"
    def mT__89(self, ):
        try:
            _type = T__89
            _channel = DEFAULT_CHANNEL

            # osekoil.g:24:7: ( 'CPU' )
            # osekoil.g:24:9: 'CPU'
            pass 
            self.match("CPU")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__89"



    # $ANTLR start "T__90"
    def mT__90(self, ):
        try:
            _type = T__90
            _channel = DEFAULT_CHANNEL

            # osekoil.g:25:7: ( 'ENUM' )
            # osekoil.g:25:9: 'ENUM'
            pass 
            self.match("ENUM")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__90"



    # $ANTLR start "T__91"
    def mT__91(self, ):
        try:
            _type = T__91
            _channel = DEFAULT_CHANNEL

            # osekoil.g:26:7: ( 'EVENT' )
            # osekoil.g:26:9: 'EVENT'
            pass 
            self.match("EVENT")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__91"



    # $ANTLR start "T__92"
    def mT__92(self, ):
        try:
            _type = T__92
            _channel = DEFAULT_CHANNEL

            # osekoil.g:27:7: ( 'EVENT_TYPE' )
            # osekoil.g:27:9: 'EVENT_TYPE'
            pass 
            self.match("EVENT_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__92"



    # $ANTLR start "T__93"
    def mT__93(self, ):
        try:
            _type = T__93
            _channel = DEFAULT_CHANNEL

            # osekoil.g:28:7: ( 'FALSE' )
            # osekoil.g:28:9: 'FALSE'
            pass 
            self.match("FALSE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__93"



    # $ANTLR start "T__94"
    def mT__94(self, ):
        try:
            _type = T__94
            _channel = DEFAULT_CHANNEL

            # osekoil.g:29:7: ( 'FLOAT' )
            # osekoil.g:29:9: 'FLOAT'
            pass 
            self.match("FLOAT")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__94"



    # $ANTLR start "T__95"
    def mT__95(self, ):
        try:
            _type = T__95
            _channel = DEFAULT_CHANNEL

            # osekoil.g:30:7: ( 'IMPLEMENTATION' )
            # osekoil.g:30:9: 'IMPLEMENTATION'
            pass 
            self.match("IMPLEMENTATION")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__95"



    # $ANTLR start "T__96"
    def mT__96(self, ):
        try:
            _type = T__96
            _channel = DEFAULT_CHANNEL

            # osekoil.g:31:7: ( 'INT32' )
            # osekoil.g:31:9: 'INT32'
            pass 
            self.match("INT32")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__96"



    # $ANTLR start "T__97"
    def mT__97(self, ):
        try:
            _type = T__97
            _channel = DEFAULT_CHANNEL

            # osekoil.g:32:7: ( 'INT64' )
            # osekoil.g:32:9: 'INT64'
            pass 
            self.match("INT64")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__97"



    # $ANTLR start "T__98"
    def mT__98(self, ):
        try:
            _type = T__98
            _channel = DEFAULT_CHANNEL

            # osekoil.g:33:7: ( 'IPDU' )
            # osekoil.g:33:9: 'IPDU'
            pass 
            self.match("IPDU")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__98"



    # $ANTLR start "T__99"
    def mT__99(self, ):
        try:
            _type = T__99
            _channel = DEFAULT_CHANNEL

            # osekoil.g:34:7: ( 'IPDU_TYPE' )
            # osekoil.g:34:9: 'IPDU_TYPE'
            pass 
            self.match("IPDU_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__99"



    # $ANTLR start "T__100"
    def mT__100(self, ):
        try:
            _type = T__100
            _channel = DEFAULT_CHANNEL

            # osekoil.g:35:8: ( 'ISR' )
            # osekoil.g:35:10: 'ISR'
            pass 
            self.match("ISR")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__100"



    # $ANTLR start "T__101"
    def mT__101(self, ):
        try:
            _type = T__101
            _channel = DEFAULT_CHANNEL

            # osekoil.g:36:8: ( 'ISR_TYPE' )
            # osekoil.g:36:10: 'ISR_TYPE'
            pass 
            self.match("ISR_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__101"



    # $ANTLR start "T__102"
    def mT__102(self, ):
        try:
            _type = T__102
            _channel = DEFAULT_CHANNEL

            # osekoil.g:37:8: ( 'MESSAGE' )
            # osekoil.g:37:10: 'MESSAGE'
            pass 
            self.match("MESSAGE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__102"



    # $ANTLR start "T__103"
    def mT__103(self, ):
        try:
            _type = T__103
            _channel = DEFAULT_CHANNEL

            # osekoil.g:38:8: ( 'MESSAGE_TYPE' )
            # osekoil.g:38:10: 'MESSAGE_TYPE'
            pass 
            self.match("MESSAGE_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__103"



    # $ANTLR start "T__104"
    def mT__104(self, ):
        try:
            _type = T__104
            _channel = DEFAULT_CHANNEL

            # osekoil.g:39:8: ( 'NETWORKMESSAGE' )
            # osekoil.g:39:10: 'NETWORKMESSAGE'
            pass 
            self.match("NETWORKMESSAGE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__104"



    # $ANTLR start "T__105"
    def mT__105(self, ):
        try:
            _type = T__105
            _channel = DEFAULT_CHANNEL

            # osekoil.g:40:8: ( 'NETWORKMESSAGE_TYPE' )
            # osekoil.g:40:10: 'NETWORKMESSAGE_TYPE'
            pass 
            self.match("NETWORKMESSAGE_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__105"



    # $ANTLR start "T__106"
    def mT__106(self, ):
        try:
            _type = T__106
            _channel = DEFAULT_CHANNEL

            # osekoil.g:41:8: ( 'NM' )
            # osekoil.g:41:10: 'NM'
            pass 
            self.match("NM")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__106"



    # $ANTLR start "T__107"
    def mT__107(self, ):
        try:
            _type = T__107
            _channel = DEFAULT_CHANNEL

            # osekoil.g:42:8: ( 'NM_TYPE' )
            # osekoil.g:42:10: 'NM_TYPE'
            pass 
            self.match("NM_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__107"



    # $ANTLR start "T__108"
    def mT__108(self, ):
        try:
            _type = T__108
            _channel = DEFAULT_CHANNEL

            # osekoil.g:43:8: ( 'NO_DEFAULT' )
            # osekoil.g:43:10: 'NO_DEFAULT'
            pass 
            self.match("NO_DEFAULT")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__108"



    # $ANTLR start "T__109"
    def mT__109(self, ):
        try:
            _type = T__109
            _channel = DEFAULT_CHANNEL

            # osekoil.g:44:8: ( 'OIL_VERSION' )
            # osekoil.g:44:10: 'OIL_VERSION'
            pass 
            self.match("OIL_VERSION")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__109"



    # $ANTLR start "T__110"
    def mT__110(self, ):
        try:
            _type = T__110
            _channel = DEFAULT_CHANNEL

            # osekoil.g:45:8: ( 'OS' )
            # osekoil.g:45:10: 'OS'
            pass 
            self.match("OS")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__110"



    # $ANTLR start "T__111"
    def mT__111(self, ):
        try:
            _type = T__111
            _channel = DEFAULT_CHANNEL

            # osekoil.g:46:8: ( 'OS_TYPE' )
            # osekoil.g:46:10: 'OS_TYPE'
            pass 
            self.match("OS_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__111"



    # $ANTLR start "T__112"
    def mT__112(self, ):
        try:
            _type = T__112
            _channel = DEFAULT_CHANNEL

            # osekoil.g:47:8: ( 'RESOURCE' )
            # osekoil.g:47:10: 'RESOURCE'
            pass 
            self.match("RESOURCE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__112"



    # $ANTLR start "T__113"
    def mT__113(self, ):
        try:
            _type = T__113
            _channel = DEFAULT_CHANNEL

            # osekoil.g:48:8: ( 'RESOURCE_TYPE' )
            # osekoil.g:48:10: 'RESOURCE_TYPE'
            pass 
            self.match("RESOURCE_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__113"



    # $ANTLR start "T__114"
    def mT__114(self, ):
        try:
            _type = T__114
            _channel = DEFAULT_CHANNEL

            # osekoil.g:49:8: ( 'STRING' )
            # osekoil.g:49:10: 'STRING'
            pass 
            self.match("STRING")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__114"



    # $ANTLR start "T__115"
    def mT__115(self, ):
        try:
            _type = T__115
            _channel = DEFAULT_CHANNEL

            # osekoil.g:50:8: ( 'TASK' )
            # osekoil.g:50:10: 'TASK'
            pass 
            self.match("TASK")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__115"



    # $ANTLR start "T__116"
    def mT__116(self, ):
        try:
            _type = T__116
            _channel = DEFAULT_CHANNEL

            # osekoil.g:51:8: ( 'TASK_TYPE' )
            # osekoil.g:51:10: 'TASK_TYPE'
            pass 
            self.match("TASK_TYPE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__116"



    # $ANTLR start "T__117"
    def mT__117(self, ):
        try:
            _type = T__117
            _channel = DEFAULT_CHANNEL

            # osekoil.g:52:8: ( 'TRUE' )
            # osekoil.g:52:10: 'TRUE'
            pass 
            self.match("TRUE")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__117"



    # $ANTLR start "T__118"
    def mT__118(self, ):
        try:
            _type = T__118
            _channel = DEFAULT_CHANNEL

            # osekoil.g:53:8: ( 'UINT32' )
            # osekoil.g:53:10: 'UINT32'
            pass 
            self.match("UINT32")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__118"



    # $ANTLR start "T__119"
    def mT__119(self, ):
        try:
            _type = T__119
            _channel = DEFAULT_CHANNEL

            # osekoil.g:54:8: ( 'UINT64' )
            # osekoil.g:54:10: 'UINT64'
            pass 
            self.match("UINT64")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__119"



    # $ANTLR start "T__120"
    def mT__120(self, ):
        try:
            _type = T__120
            _channel = DEFAULT_CHANNEL

            # osekoil.g:55:8: ( 'WITH_AUTO' )
            # osekoil.g:55:10: 'WITH_AUTO'
            pass 
            self.match("WITH_AUTO")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__120"



    # $ANTLR start "ID"
    def mID(self, ):
        try:
            _type = ID
            _channel = DEFAULT_CHANNEL

            # osekoil.g:719:5: ( ( 'a' .. 'z' | 'A' .. 'Z' | '_' ) ( 'a' .. 'z' | 'A' .. 'Z' | '0' .. '9' | '_' )* )
            # osekoil.g:719:7: ( 'a' .. 'z' | 'A' .. 'Z' | '_' ) ( 'a' .. 'z' | 'A' .. 'Z' | '0' .. '9' | '_' )*
            pass 
            if (65 <= self.input.LA(1) <= 90) or self.input.LA(1) == 95 or (97 <= self.input.LA(1) <= 122):
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse



            # osekoil.g:719:31: ( 'a' .. 'z' | 'A' .. 'Z' | '0' .. '9' | '_' )*
            while True: #loop1
                alt1 = 2
                LA1_0 = self.input.LA(1)

                if ((48 <= LA1_0 <= 57) or (65 <= LA1_0 <= 90) or LA1_0 == 95 or (97 <= LA1_0 <= 122)) :
                    alt1 = 1


                if alt1 == 1:
                    # osekoil.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57) or (65 <= self.input.LA(1) <= 90) or self.input.LA(1) == 95 or (97 <= self.input.LA(1) <= 122):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    break #loop1




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "ID"



    # $ANTLR start "INT"
    def mINT(self, ):
        try:
            _type = INT
            _channel = DEFAULT_CHANNEL

            # osekoil.g:722:5: ( ( '0' .. '9' )+ )
            # osekoil.g:722:7: ( '0' .. '9' )+
            pass 
            # osekoil.g:722:7: ( '0' .. '9' )+
            cnt2 = 0
            while True: #loop2
                alt2 = 2
                LA2_0 = self.input.LA(1)

                if ((48 <= LA2_0 <= 57)) :
                    alt2 = 1


                if alt2 == 1:
                    # osekoil.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    if cnt2 >= 1:
                        break #loop2

                    eee = EarlyExitException(2, self.input)
                    raise eee

                cnt2 += 1




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "INT"



    # $ANTLR start "FLOAT"
    def mFLOAT(self, ):
        try:
            _type = FLOAT
            _channel = DEFAULT_CHANNEL

            # osekoil.g:726:5: ( ( '0' .. '9' )+ '.' ( '0' .. '9' )* ( EXPONENT )? | '.' ( '0' .. '9' )+ ( EXPONENT )? | ( '0' .. '9' )+ EXPONENT )
            alt9 = 3
            alt9 = self.dfa9.predict(self.input)
            if alt9 == 1:
                # osekoil.g:726:9: ( '0' .. '9' )+ '.' ( '0' .. '9' )* ( EXPONENT )?
                pass 
                # osekoil.g:726:9: ( '0' .. '9' )+
                cnt3 = 0
                while True: #loop3
                    alt3 = 2
                    LA3_0 = self.input.LA(1)

                    if ((48 <= LA3_0 <= 57)) :
                        alt3 = 1


                    if alt3 == 1:
                        # osekoil.g:
                        pass 
                        if (48 <= self.input.LA(1) <= 57):
                            self.input.consume()
                        else:
                            mse = MismatchedSetException(None, self.input)
                            self.recover(mse)
                            raise mse




                    else:
                        if cnt3 >= 1:
                            break #loop3

                        eee = EarlyExitException(3, self.input)
                        raise eee

                    cnt3 += 1


                self.match(46)

                # osekoil.g:726:25: ( '0' .. '9' )*
                while True: #loop4
                    alt4 = 2
                    LA4_0 = self.input.LA(1)

                    if ((48 <= LA4_0 <= 57)) :
                        alt4 = 1


                    if alt4 == 1:
                        # osekoil.g:
                        pass 
                        if (48 <= self.input.LA(1) <= 57):
                            self.input.consume()
                        else:
                            mse = MismatchedSetException(None, self.input)
                            self.recover(mse)
                            raise mse




                    else:
                        break #loop4


                # osekoil.g:726:37: ( EXPONENT )?
                alt5 = 2
                LA5_0 = self.input.LA(1)

                if (LA5_0 == 69 or LA5_0 == 101) :
                    alt5 = 1
                if alt5 == 1:
                    # osekoil.g:726:37: EXPONENT
                    pass 
                    self.mEXPONENT()






            elif alt9 == 2:
                # osekoil.g:727:9: '.' ( '0' .. '9' )+ ( EXPONENT )?
                pass 
                self.match(46)

                # osekoil.g:727:13: ( '0' .. '9' )+
                cnt6 = 0
                while True: #loop6
                    alt6 = 2
                    LA6_0 = self.input.LA(1)

                    if ((48 <= LA6_0 <= 57)) :
                        alt6 = 1


                    if alt6 == 1:
                        # osekoil.g:
                        pass 
                        if (48 <= self.input.LA(1) <= 57):
                            self.input.consume()
                        else:
                            mse = MismatchedSetException(None, self.input)
                            self.recover(mse)
                            raise mse




                    else:
                        if cnt6 >= 1:
                            break #loop6

                        eee = EarlyExitException(6, self.input)
                        raise eee

                    cnt6 += 1


                # osekoil.g:727:25: ( EXPONENT )?
                alt7 = 2
                LA7_0 = self.input.LA(1)

                if (LA7_0 == 69 or LA7_0 == 101) :
                    alt7 = 1
                if alt7 == 1:
                    # osekoil.g:727:25: EXPONENT
                    pass 
                    self.mEXPONENT()






            elif alt9 == 3:
                # osekoil.g:728:9: ( '0' .. '9' )+ EXPONENT
                pass 
                # osekoil.g:728:9: ( '0' .. '9' )+
                cnt8 = 0
                while True: #loop8
                    alt8 = 2
                    LA8_0 = self.input.LA(1)

                    if ((48 <= LA8_0 <= 57)) :
                        alt8 = 1


                    if alt8 == 1:
                        # osekoil.g:
                        pass 
                        if (48 <= self.input.LA(1) <= 57):
                            self.input.consume()
                        else:
                            mse = MismatchedSetException(None, self.input)
                            self.recover(mse)
                            raise mse




                    else:
                        if cnt8 >= 1:
                            break #loop8

                        eee = EarlyExitException(8, self.input)
                        raise eee

                    cnt8 += 1


                self.mEXPONENT()



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "FLOAT"



    # $ANTLR start "LINE_NUMBER"
    def mLINE_NUMBER(self, ):
        try:
            _type = LINE_NUMBER
            _channel = DEFAULT_CHANNEL

            STRING1 = None

            # osekoil.g:731:12: ( '#line ' INT ' ' STRING )
            # osekoil.g:732:5: '#line ' INT ' ' STRING
            pass 
            self.match("#line ")


            #action start
            print self.input.LT(1) 
            #action end


            self.mINT()


            self.match(32)

            STRING1Start529 = self.getCharIndex()
            self.mSTRING()
            STRING1StartLine529 = self.getLine()
            STRING1StartCharPos529 = self.getCharPositionInLine()
            STRING1 = CommonToken(
                input=self.input,
                type=INVALID_TOKEN_TYPE,
                channel=DEFAULT_CHANNEL,
                start=STRING1Start529,
                stop=self.getCharIndex()-1)
            STRING1.setLine(STRING1StartLine529)
            STRING1.setCharPositionInLine(STRING1StartCharPos529)


            #action start
            print ((STRING1 is not None) and [STRING1.text] or [None])[0] 
            #action end




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "LINE_NUMBER"



    # $ANTLR start "SIGN"
    def mSIGN(self, ):
        try:
            # osekoil.g:739:5: ( () | '+' | '-' )
            alt10 = 3
            LA10 = self.input.LA(1)
            if LA10 == 43:
                alt10 = 2
            elif LA10 == 45:
                alt10 = 3
            else:
                alt10 = 1

            if alt10 == 1:
                # osekoil.g:739:7: ()
                pass 
                # osekoil.g:739:7: ()
                # osekoil.g:739:8: 
                pass 




            elif alt10 == 2:
                # osekoil.g:739:13: '+'
                pass 
                self.match(43)


            elif alt10 == 3:
                # osekoil.g:739:19: '-'
                pass 
                self.match(45)



        finally:
            pass

    # $ANTLR end "SIGN"



    # $ANTLR start "DECDIGITS"
    def mDECDIGITS(self, ):
        try:
            # osekoil.g:743:4: ( ZERODIGIT POSDIGIT )
            # osekoil.g:743:7: ZERODIGIT POSDIGIT
            pass 
            self.mZERODIGIT()


            self.mPOSDIGIT()





        finally:
            pass

    # $ANTLR end "DECDIGITS"



    # $ANTLR start "SEMI"
    def mSEMI(self, ):
        try:
            _type = SEMI
            _channel = DEFAULT_CHANNEL

            # osekoil.g:746:4: ( ';' )
            # osekoil.g:746:7: ';'
            pass 
            self.match(59)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "SEMI"



    # $ANTLR start "ASSIGN"
    def mASSIGN(self, ):
        try:
            _type = ASSIGN
            _channel = DEFAULT_CHANNEL

            # osekoil.g:749:4: ( '=' )
            # osekoil.g:749:7: '='
            pass 
            self.match(61)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "ASSIGN"



    # $ANTLR start "LCURLY"
    def mLCURLY(self, ):
        try:
            _type = LCURLY
            _channel = DEFAULT_CHANNEL

            # osekoil.g:752:4: ( '{' )
            # osekoil.g:752:7: '{'
            pass 
            self.match(123)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "LCURLY"



    # $ANTLR start "RCURLY"
    def mRCURLY(self, ):
        try:
            _type = RCURLY
            _channel = DEFAULT_CHANNEL

            # osekoil.g:755:4: ( '}' )
            # osekoil.g:755:7: '}'
            pass 
            self.match(125)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "RCURLY"



    # $ANTLR start "LPAREN"
    def mLPAREN(self, ):
        try:
            _type = LPAREN
            _channel = DEFAULT_CHANNEL

            # osekoil.g:758:4: ( '(' )
            # osekoil.g:758:7: '('
            pass 
            self.match(40)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "LPAREN"



    # $ANTLR start "RRAREN"
    def mRRAREN(self, ):
        try:
            _type = RRAREN
            _channel = DEFAULT_CHANNEL

            # osekoil.g:761:4: ( ')' )
            # osekoil.g:761:7: ')'
            pass 
            self.match(41)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "RRAREN"



    # $ANTLR start "LBRACK"
    def mLBRACK(self, ):
        try:
            _type = LBRACK
            _channel = DEFAULT_CHANNEL

            # osekoil.g:764:4: ( '[' )
            # osekoil.g:764:7: '['
            pass 
            self.match(91)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "LBRACK"



    # $ANTLR start "RBRACK"
    def mRBRACK(self, ):
        try:
            _type = RBRACK
            _channel = DEFAULT_CHANNEL

            # osekoil.g:768:4: ( ']' )
            # osekoil.g:768:7: ']'
            pass 
            self.match(93)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "RBRACK"



    # $ANTLR start "ZERODIGIT"
    def mZERODIGIT(self, ):
        try:
            # osekoil.g:773:4: ( '0' )
            # osekoil.g:773:6: '0'
            pass 
            self.match(48)




        finally:
            pass

    # $ANTLR end "ZERODIGIT"



    # $ANTLR start "POSDIGIT"
    def mPOSDIGIT(self, ):
        try:
            # osekoil.g:778:4: ( '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' )
            # osekoil.g:
            pass 
            if (49 <= self.input.LA(1) <= 57):
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse






        finally:
            pass

    # $ANTLR end "POSDIGIT"



    # $ANTLR start "HEX"
    def mHEX(self, ):
        try:
            _type = HEX
            _channel = DEFAULT_CHANNEL

            # osekoil.g:781:5: ( '0' ( 'x' | 'X' ) ( HEX_DIGIT )+ )
            # osekoil.g:781:8: '0' ( 'x' | 'X' ) ( HEX_DIGIT )+
            pass 
            self.match(48)

            if self.input.LA(1) == 88 or self.input.LA(1) == 120:
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse



            # osekoil.g:781:23: ( HEX_DIGIT )+
            cnt11 = 0
            while True: #loop11
                alt11 = 2
                LA11_0 = self.input.LA(1)

                if ((48 <= LA11_0 <= 57) or (65 <= LA11_0 <= 70) or (97 <= LA11_0 <= 102)) :
                    alt11 = 1


                if alt11 == 1:
                    # osekoil.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57) or (65 <= self.input.LA(1) <= 70) or (97 <= self.input.LA(1) <= 102):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    if cnt11 >= 1:
                        break #loop11

                    eee = EarlyExitException(11, self.input)
                    raise eee

                cnt11 += 1




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "HEX"



    # $ANTLR start "COMMENT"
    def mCOMMENT(self, ):
        try:
            _type = COMMENT
            _channel = DEFAULT_CHANNEL

            # osekoil.g:785:5: ( '//' (~ ( '\\n' | '\\r' ) )* ( '\\r' )? '\\n' | '/*' ( options {greedy=false; } : . )* '*/' )
            alt15 = 2
            LA15_0 = self.input.LA(1)

            if (LA15_0 == 47) :
                LA15_1 = self.input.LA(2)

                if (LA15_1 == 47) :
                    alt15 = 1
                elif (LA15_1 == 42) :
                    alt15 = 2
                else:
                    nvae = NoViableAltException("", 15, 1, self.input)

                    raise nvae


            else:
                nvae = NoViableAltException("", 15, 0, self.input)

                raise nvae


            if alt15 == 1:
                # osekoil.g:785:9: '//' (~ ( '\\n' | '\\r' ) )* ( '\\r' )? '\\n'
                pass 
                self.match("//")


                # osekoil.g:785:14: (~ ( '\\n' | '\\r' ) )*
                while True: #loop12
                    alt12 = 2
                    LA12_0 = self.input.LA(1)

                    if ((0 <= LA12_0 <= 9) or (11 <= LA12_0 <= 12) or (14 <= LA12_0 <= 65535)) :
                        alt12 = 1


                    if alt12 == 1:
                        # osekoil.g:
                        pass 
                        if (0 <= self.input.LA(1) <= 9) or (11 <= self.input.LA(1) <= 12) or (14 <= self.input.LA(1) <= 65535):
                            self.input.consume()
                        else:
                            mse = MismatchedSetException(None, self.input)
                            self.recover(mse)
                            raise mse




                    else:
                        break #loop12


                # osekoil.g:785:28: ( '\\r' )?
                alt13 = 2
                LA13_0 = self.input.LA(1)

                if (LA13_0 == 13) :
                    alt13 = 1
                if alt13 == 1:
                    # osekoil.g:785:28: '\\r'
                    pass 
                    self.match(13)




                self.match(10)

                #action start
                _channel=HIDDEN;
                #action end



            elif alt15 == 2:
                # osekoil.g:786:9: '/*' ( options {greedy=false; } : . )* '*/'
                pass 
                self.match("/*")


                # osekoil.g:786:14: ( options {greedy=false; } : . )*
                while True: #loop14
                    alt14 = 2
                    LA14_0 = self.input.LA(1)

                    if (LA14_0 == 42) :
                        LA14_1 = self.input.LA(2)

                        if (LA14_1 == 47) :
                            alt14 = 2
                        elif ((0 <= LA14_1 <= 46) or (48 <= LA14_1 <= 65535)) :
                            alt14 = 1


                    elif ((0 <= LA14_0 <= 41) or (43 <= LA14_0 <= 65535)) :
                        alt14 = 1


                    if alt14 == 1:
                        # osekoil.g:786:42: .
                        pass 
                        self.matchAny()


                    else:
                        break #loop14


                self.match("*/")


                #action start
                _channel=HIDDEN;
                #action end



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "COMMENT"



    # $ANTLR start "WS"
    def mWS(self, ):
        try:
            _type = WS
            _channel = DEFAULT_CHANNEL

            # osekoil.g:789:5: ( ( ' ' | '\\t' | '\\r' | '\\n' ) )
            # osekoil.g:790:5: ( ' ' | '\\t' | '\\r' | '\\n' )
            pass 
            if (9 <= self.input.LA(1) <= 10) or self.input.LA(1) == 13 or self.input.LA(1) == 32:
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse



            #action start
            _channel=HIDDEN; 
            #action end




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "WS"



    # $ANTLR start "STRING"
    def mSTRING(self, ):
        try:
            _type = STRING
            _channel = DEFAULT_CHANNEL

            # osekoil.g:799:5: ( '\"' ( ESC_SEQ |~ ( '\\\\' | '\"' ) )* '\"' )
            # osekoil.g:799:8: '\"' ( ESC_SEQ |~ ( '\\\\' | '\"' ) )* '\"'
            pass 
            self.match(34)

            # osekoil.g:799:12: ( ESC_SEQ |~ ( '\\\\' | '\"' ) )*
            while True: #loop16
                alt16 = 3
                LA16_0 = self.input.LA(1)

                if (LA16_0 == 92) :
                    alt16 = 1
                elif ((0 <= LA16_0 <= 33) or (35 <= LA16_0 <= 91) or (93 <= LA16_0 <= 65535)) :
                    alt16 = 2


                if alt16 == 1:
                    # osekoil.g:799:14: ESC_SEQ
                    pass 
                    self.mESC_SEQ()



                elif alt16 == 2:
                    # osekoil.g:799:24: ~ ( '\\\\' | '\"' )
                    pass 
                    if (0 <= self.input.LA(1) <= 33) or (35 <= self.input.LA(1) <= 91) or (93 <= self.input.LA(1) <= 65535):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    break #loop16


            self.match(34)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "STRING"



    # $ANTLR start "EXPONENT"
    def mEXPONENT(self, ):
        try:
            # osekoil.g:804:10: ( ( 'e' | 'E' ) ( '+' | '-' )? ( '0' .. '9' )+ )
            # osekoil.g:804:12: ( 'e' | 'E' ) ( '+' | '-' )? ( '0' .. '9' )+
            pass 
            if self.input.LA(1) == 69 or self.input.LA(1) == 101:
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse



            # osekoil.g:804:22: ( '+' | '-' )?
            alt17 = 2
            LA17_0 = self.input.LA(1)

            if (LA17_0 == 43 or LA17_0 == 45) :
                alt17 = 1
            if alt17 == 1:
                # osekoil.g:
                pass 
                if self.input.LA(1) == 43 or self.input.LA(1) == 45:
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse






            # osekoil.g:804:33: ( '0' .. '9' )+
            cnt18 = 0
            while True: #loop18
                alt18 = 2
                LA18_0 = self.input.LA(1)

                if ((48 <= LA18_0 <= 57)) :
                    alt18 = 1


                if alt18 == 1:
                    # osekoil.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    if cnt18 >= 1:
                        break #loop18

                    eee = EarlyExitException(18, self.input)
                    raise eee

                cnt18 += 1





        finally:
            pass

    # $ANTLR end "EXPONENT"



    # $ANTLR start "HEX_DIGIT"
    def mHEX_DIGIT(self, ):
        try:
            # osekoil.g:808:11: ( ( '0' .. '9' | 'a' .. 'f' | 'A' .. 'F' ) )
            # osekoil.g:
            pass 
            if (48 <= self.input.LA(1) <= 57) or (65 <= self.input.LA(1) <= 70) or (97 <= self.input.LA(1) <= 102):
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse






        finally:
            pass

    # $ANTLR end "HEX_DIGIT"



    # $ANTLR start "ESC_SEQ"
    def mESC_SEQ(self, ):
        try:
            # osekoil.g:813:5: ( '\\\\' ( 'b' | 't' | 'n' | 'f' | 'r' | '\\\"' | '\\'' | '\\\\' ) | UNICODE_ESC | OCTAL_ESC )
            alt19 = 3
            LA19_0 = self.input.LA(1)

            if (LA19_0 == 92) :
                LA19 = self.input.LA(2)
                if LA19 == 34 or LA19 == 39 or LA19 == 92 or LA19 == 98 or LA19 == 102 or LA19 == 110 or LA19 == 114 or LA19 == 116:
                    alt19 = 1
                elif LA19 == 117:
                    alt19 = 2
                elif LA19 == 48 or LA19 == 49 or LA19 == 50 or LA19 == 51 or LA19 == 52 or LA19 == 53 or LA19 == 54 or LA19 == 55:
                    alt19 = 3
                else:
                    nvae = NoViableAltException("", 19, 1, self.input)

                    raise nvae


            else:
                nvae = NoViableAltException("", 19, 0, self.input)

                raise nvae


            if alt19 == 1:
                # osekoil.g:813:9: '\\\\' ( 'b' | 't' | 'n' | 'f' | 'r' | '\\\"' | '\\'' | '\\\\' )
                pass 
                self.match(92)

                if self.input.LA(1) == 34 or self.input.LA(1) == 39 or self.input.LA(1) == 92 or self.input.LA(1) == 98 or self.input.LA(1) == 102 or self.input.LA(1) == 110 or self.input.LA(1) == 114 or self.input.LA(1) == 116:
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse




            elif alt19 == 2:
                # osekoil.g:814:9: UNICODE_ESC
                pass 
                self.mUNICODE_ESC()



            elif alt19 == 3:
                # osekoil.g:815:9: OCTAL_ESC
                pass 
                self.mOCTAL_ESC()




        finally:
            pass

    # $ANTLR end "ESC_SEQ"



    # $ANTLR start "OCTAL_ESC"
    def mOCTAL_ESC(self, ):
        try:
            # osekoil.g:820:5: ( '\\\\' ( '0' .. '3' ) ( '0' .. '7' ) ( '0' .. '7' ) | '\\\\' ( '0' .. '7' ) ( '0' .. '7' ) | '\\\\' ( '0' .. '7' ) )
            alt20 = 3
            LA20_0 = self.input.LA(1)

            if (LA20_0 == 92) :
                LA20_1 = self.input.LA(2)

                if ((48 <= LA20_1 <= 51)) :
                    LA20_2 = self.input.LA(3)

                    if ((48 <= LA20_2 <= 55)) :
                        LA20_4 = self.input.LA(4)

                        if ((48 <= LA20_4 <= 55)) :
                            alt20 = 1
                        else:
                            alt20 = 2

                    else:
                        alt20 = 3

                elif ((52 <= LA20_1 <= 55)) :
                    LA20_3 = self.input.LA(3)

                    if ((48 <= LA20_3 <= 55)) :
                        alt20 = 2
                    else:
                        alt20 = 3

                else:
                    nvae = NoViableAltException("", 20, 1, self.input)

                    raise nvae


            else:
                nvae = NoViableAltException("", 20, 0, self.input)

                raise nvae


            if alt20 == 1:
                # osekoil.g:820:9: '\\\\' ( '0' .. '3' ) ( '0' .. '7' ) ( '0' .. '7' )
                pass 
                self.match(92)

                if (48 <= self.input.LA(1) <= 51):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse



                if (48 <= self.input.LA(1) <= 55):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse



                if (48 <= self.input.LA(1) <= 55):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse




            elif alt20 == 2:
                # osekoil.g:821:9: '\\\\' ( '0' .. '7' ) ( '0' .. '7' )
                pass 
                self.match(92)

                if (48 <= self.input.LA(1) <= 55):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse



                if (48 <= self.input.LA(1) <= 55):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse




            elif alt20 == 3:
                # osekoil.g:822:9: '\\\\' ( '0' .. '7' )
                pass 
                self.match(92)

                if (48 <= self.input.LA(1) <= 55):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse





        finally:
            pass

    # $ANTLR end "OCTAL_ESC"



    # $ANTLR start "UNICODE_ESC"
    def mUNICODE_ESC(self, ):
        try:
            # osekoil.g:827:5: ( '\\\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT )
            # osekoil.g:827:9: '\\\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
            pass 
            self.match(92)

            self.match(117)

            self.mHEX_DIGIT()


            self.mHEX_DIGIT()


            self.mHEX_DIGIT()


            self.mHEX_DIGIT()





        finally:
            pass

    # $ANTLR end "UNICODE_ESC"



    def mTokens(self):
        # osekoil.g:1:8: ( T__76 | T__77 | T__78 | T__79 | T__80 | T__81 | T__82 | T__83 | T__84 | T__85 | T__86 | T__87 | T__88 | T__89 | T__90 | T__91 | T__92 | T__93 | T__94 | T__95 | T__96 | T__97 | T__98 | T__99 | T__100 | T__101 | T__102 | T__103 | T__104 | T__105 | T__106 | T__107 | T__108 | T__109 | T__110 | T__111 | T__112 | T__113 | T__114 | T__115 | T__116 | T__117 | T__118 | T__119 | T__120 | ID | INT | FLOAT | LINE_NUMBER | SEMI | ASSIGN | LCURLY | RCURLY | LPAREN | RRAREN | LBRACK | RBRACK | HEX | COMMENT | WS | STRING )
        alt21 = 61
        alt21 = self.dfa21.predict(self.input)
        if alt21 == 1:
            # osekoil.g:1:10: T__76
            pass 
            self.mT__76()



        elif alt21 == 2:
            # osekoil.g:1:16: T__77
            pass 
            self.mT__77()



        elif alt21 == 3:
            # osekoil.g:1:22: T__78
            pass 
            self.mT__78()



        elif alt21 == 4:
            # osekoil.g:1:28: T__79
            pass 
            self.mT__79()



        elif alt21 == 5:
            # osekoil.g:1:34: T__80
            pass 
            self.mT__80()



        elif alt21 == 6:
            # osekoil.g:1:40: T__81
            pass 
            self.mT__81()



        elif alt21 == 7:
            # osekoil.g:1:46: T__82
            pass 
            self.mT__82()



        elif alt21 == 8:
            # osekoil.g:1:52: T__83
            pass 
            self.mT__83()



        elif alt21 == 9:
            # osekoil.g:1:58: T__84
            pass 
            self.mT__84()



        elif alt21 == 10:
            # osekoil.g:1:64: T__85
            pass 
            self.mT__85()



        elif alt21 == 11:
            # osekoil.g:1:70: T__86
            pass 
            self.mT__86()



        elif alt21 == 12:
            # osekoil.g:1:76: T__87
            pass 
            self.mT__87()



        elif alt21 == 13:
            # osekoil.g:1:82: T__88
            pass 
            self.mT__88()



        elif alt21 == 14:
            # osekoil.g:1:88: T__89
            pass 
            self.mT__89()



        elif alt21 == 15:
            # osekoil.g:1:94: T__90
            pass 
            self.mT__90()



        elif alt21 == 16:
            # osekoil.g:1:100: T__91
            pass 
            self.mT__91()



        elif alt21 == 17:
            # osekoil.g:1:106: T__92
            pass 
            self.mT__92()



        elif alt21 == 18:
            # osekoil.g:1:112: T__93
            pass 
            self.mT__93()



        elif alt21 == 19:
            # osekoil.g:1:118: T__94
            pass 
            self.mT__94()



        elif alt21 == 20:
            # osekoil.g:1:124: T__95
            pass 
            self.mT__95()



        elif alt21 == 21:
            # osekoil.g:1:130: T__96
            pass 
            self.mT__96()



        elif alt21 == 22:
            # osekoil.g:1:136: T__97
            pass 
            self.mT__97()



        elif alt21 == 23:
            # osekoil.g:1:142: T__98
            pass 
            self.mT__98()



        elif alt21 == 24:
            # osekoil.g:1:148: T__99
            pass 
            self.mT__99()



        elif alt21 == 25:
            # osekoil.g:1:154: T__100
            pass 
            self.mT__100()



        elif alt21 == 26:
            # osekoil.g:1:161: T__101
            pass 
            self.mT__101()



        elif alt21 == 27:
            # osekoil.g:1:168: T__102
            pass 
            self.mT__102()



        elif alt21 == 28:
            # osekoil.g:1:175: T__103
            pass 
            self.mT__103()



        elif alt21 == 29:
            # osekoil.g:1:182: T__104
            pass 
            self.mT__104()



        elif alt21 == 30:
            # osekoil.g:1:189: T__105
            pass 
            self.mT__105()



        elif alt21 == 31:
            # osekoil.g:1:196: T__106
            pass 
            self.mT__106()



        elif alt21 == 32:
            # osekoil.g:1:203: T__107
            pass 
            self.mT__107()



        elif alt21 == 33:
            # osekoil.g:1:210: T__108
            pass 
            self.mT__108()



        elif alt21 == 34:
            # osekoil.g:1:217: T__109
            pass 
            self.mT__109()



        elif alt21 == 35:
            # osekoil.g:1:224: T__110
            pass 
            self.mT__110()



        elif alt21 == 36:
            # osekoil.g:1:231: T__111
            pass 
            self.mT__111()



        elif alt21 == 37:
            # osekoil.g:1:238: T__112
            pass 
            self.mT__112()



        elif alt21 == 38:
            # osekoil.g:1:245: T__113
            pass 
            self.mT__113()



        elif alt21 == 39:
            # osekoil.g:1:252: T__114
            pass 
            self.mT__114()



        elif alt21 == 40:
            # osekoil.g:1:259: T__115
            pass 
            self.mT__115()



        elif alt21 == 41:
            # osekoil.g:1:266: T__116
            pass 
            self.mT__116()



        elif alt21 == 42:
            # osekoil.g:1:273: T__117
            pass 
            self.mT__117()



        elif alt21 == 43:
            # osekoil.g:1:280: T__118
            pass 
            self.mT__118()



        elif alt21 == 44:
            # osekoil.g:1:287: T__119
            pass 
            self.mT__119()



        elif alt21 == 45:
            # osekoil.g:1:294: T__120
            pass 
            self.mT__120()



        elif alt21 == 46:
            # osekoil.g:1:301: ID
            pass 
            self.mID()



        elif alt21 == 47:
            # osekoil.g:1:304: INT
            pass 
            self.mINT()



        elif alt21 == 48:
            # osekoil.g:1:308: FLOAT
            pass 
            self.mFLOAT()



        elif alt21 == 49:
            # osekoil.g:1:314: LINE_NUMBER
            pass 
            self.mLINE_NUMBER()



        elif alt21 == 50:
            # osekoil.g:1:326: SEMI
            pass 
            self.mSEMI()



        elif alt21 == 51:
            # osekoil.g:1:331: ASSIGN
            pass 
            self.mASSIGN()



        elif alt21 == 52:
            # osekoil.g:1:338: LCURLY
            pass 
            self.mLCURLY()



        elif alt21 == 53:
            # osekoil.g:1:345: RCURLY
            pass 
            self.mRCURLY()



        elif alt21 == 54:
            # osekoil.g:1:352: LPAREN
            pass 
            self.mLPAREN()



        elif alt21 == 55:
            # osekoil.g:1:359: RRAREN
            pass 
            self.mRRAREN()



        elif alt21 == 56:
            # osekoil.g:1:366: LBRACK
            pass 
            self.mLBRACK()



        elif alt21 == 57:
            # osekoil.g:1:373: RBRACK
            pass 
            self.mRBRACK()



        elif alt21 == 58:
            # osekoil.g:1:380: HEX
            pass 
            self.mHEX()



        elif alt21 == 59:
            # osekoil.g:1:384: COMMENT
            pass 
            self.mCOMMENT()



        elif alt21 == 60:
            # osekoil.g:1:392: WS
            pass 
            self.mWS()



        elif alt21 == 61:
            # osekoil.g:1:395: STRING
            pass 
            self.mSTRING()








    # lookup tables for DFA #9

    DFA9_eot = DFA.unpack(
        u"\5\uffff"
        )

    DFA9_eof = DFA.unpack(
        u"\5\uffff"
        )

    DFA9_min = DFA.unpack(
        u"\2\56\3\uffff"
        )

    DFA9_max = DFA.unpack(
        u"\1\71\1\145\3\uffff"
        )

    DFA9_accept = DFA.unpack(
        u"\2\uffff\1\2\1\1\1\3"
        )

    DFA9_special = DFA.unpack(
        u"\5\uffff"
        )


    DFA9_transition = [
        DFA.unpack(u"\1\2\1\uffff\12\1"),
        DFA.unpack(u"\1\3\1\uffff\12\1\13\uffff\1\4\37\uffff\1\4"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"")
    ]

    # class definition for DFA #9

    class DFA9(DFA):
        pass


    # lookup tables for DFA #21

    DFA21_eot = DFA.unpack(
        u"\4\uffff\16\22\1\uffff\1\76\11\uffff\1\76\5\uffff\20\22\1\121\2"
        u"\22\1\125\6\22\2\uffff\4\22\1\141\1\22\1\143\7\22\1\155\3\22\1"
        u"\uffff\3\22\1\uffff\10\22\1\174\2\22\1\uffff\1\22\1\uffff\1\u0080"
        u"\6\22\1\u0088\1\22\1\uffff\10\22\1\u0093\1\u0094\2\22\1\u0099\1"
        u"\22\1\uffff\3\22\1\uffff\1\u009f\1\u00a0\1\u00a1\1\22\1\u00a3\1"
        u"\u00a4\1\22\1\uffff\12\22\2\uffff\4\22\1\uffff\5\22\3\uffff\1\22"
        u"\2\uffff\11\22\1\u00c3\1\22\1\u00c5\1\u00c6\2\22\1\u00ca\1\u00cb"
        u"\1\22\1\u00ce\4\22\1\u00d4\1\22\1\u00d6\2\22\1\u00d9\1\22\1\uffff"
        u"\1\22\2\uffff\3\22\2\uffff\1\u00df\1\22\1\uffff\3\22\1\u00e4\1"
        u"\22\1\uffff\1\22\1\uffff\2\22\1\uffff\1\u00ea\4\22\1\uffff\3\22"
        u"\1\u00f2\1\uffff\5\22\1\uffff\1\u00f8\1\u00f9\1\u00fa\2\22\1\u00fd"
        u"\1\22\1\uffff\2\22\1\u0101\2\22\3\uffff\2\22\1\uffff\3\22\1\uffff"
        u"\1\u0109\1\22\1\u010b\1\u010c\1\22\1\u010e\1\22\1\uffff\1\22\2"
        u"\uffff\1\22\1\uffff\1\22\1\u0113\1\u0114\1\u0116\2\uffff\1\22\1"
        u"\uffff\3\22\1\u011b\1\uffff"
        )

    DFA21_eof = DFA.unpack(
        u"\u011c\uffff"
        )

    DFA21_min = DFA.unpack(
        u"\1\11\1\uffff\1\56\1\uffff\1\114\2\117\1\116\1\101\1\115\2\105"
        u"\1\111\1\105\1\124\1\101\2\111\1\uffff\1\56\11\uffff\1\56\5\uffff"
        u"\1\101\1\120\1\124\1\117\1\115\2\125\1\105\1\114\1\117\1\120\1"
        u"\124\1\104\1\122\1\123\1\124\1\60\1\137\1\114\1\60\1\123\1\122"
        u"\1\123\1\125\1\116\1\124\2\uffff\1\122\1\115\1\117\1\114\1\60\1"
        u"\116\1\60\1\115\1\116\1\123\1\101\1\114\1\63\1\125\1\60\1\123\1"
        u"\127\1\124\1\uffff\1\104\1\137\1\124\1\uffff\1\117\1\111\1\113"
        u"\1\105\1\124\1\110\1\115\1\117\1\60\1\105\1\124\1\uffff\1\124\1"
        u"\uffff\1\60\1\124\1\105\1\124\1\105\1\62\1\64\1\60\1\124\1\uffff"
        u"\1\101\1\117\1\131\1\105\1\126\1\131\1\125\1\116\2\60\1\63\1\137"
        u"\1\60\1\104\1\uffff\1\101\1\131\1\105\1\uffff\3\60\1\115\2\60\1"
        u"\124\1\uffff\1\131\1\107\1\122\1\120\1\106\1\105\1\120\1\122\1"
        u"\107\1\124\2\uffff\1\62\1\64\1\101\1\124\1\uffff\1\105\1\116\1"
        u"\120\1\122\1\124\3\uffff\1\105\2\uffff\1\131\1\120\1\105\1\113"
        u"\1\105\1\101\1\122\1\105\1\103\1\60\1\131\2\60\1\125\1\131\2\60"
        u"\1\105\1\60\1\131\1\116\1\120\1\105\1\60\1\115\1\60\1\125\1\123"
        u"\1\60\1\105\1\uffff\1\120\2\uffff\1\124\1\120\1\124\2\uffff\1\60"
        u"\1\124\1\uffff\1\120\1\124\1\105\1\60\1\124\1\uffff\1\105\1\uffff"
        u"\1\114\1\111\1\uffff\1\60\1\105\1\117\1\105\1\131\1\uffff\1\131"
        u"\1\105\1\101\1\60\1\uffff\1\131\1\123\1\124\1\117\1\124\1\uffff"
        u"\3\60\2\120\1\60\1\124\1\uffff\1\120\1\123\1\60\1\116\1\131\3\uffff"
        u"\2\105\1\uffff\1\111\1\105\1\101\1\uffff\1\60\1\120\2\60\1\117"
        u"\1\60\1\107\1\uffff\1\105\2\uffff\1\116\1\uffff\1\105\3\60\2\uffff"
        u"\1\124\1\uffff\1\131\1\120\1\105\1\60\1\uffff"
        )

    DFA21_max = DFA.unpack(
        u"\1\175\1\uffff\1\71\1\uffff\1\125\1\117\1\120\1\126\1\114\1\123"
        u"\1\105\1\117\1\123\1\105\1\124\1\122\2\111\1\uffff\1\170\11\uffff"
        u"\1\145\5\uffff\1\101\1\120\1\124\1\117\3\125\1\105\1\114\1\117"
        u"\1\120\1\124\1\104\1\122\1\123\1\124\1\172\1\137\1\114\1\172\1"
        u"\123\1\122\1\123\1\125\1\116\1\124\2\uffff\1\122\1\115\1\117\1"
        u"\114\1\172\1\116\1\172\1\115\1\116\1\123\1\101\1\114\1\66\1\125"
        u"\1\172\1\123\1\127\1\124\1\uffff\1\104\1\137\1\124\1\uffff\1\117"
        u"\1\111\1\113\1\105\1\124\1\110\1\115\1\117\1\172\1\105\1\124\1"
        u"\uffff\1\124\1\uffff\1\172\1\124\1\105\1\124\1\105\1\62\1\64\1"
        u"\172\1\124\1\uffff\1\101\1\117\1\131\1\105\1\126\1\131\1\125\1"
        u"\116\2\172\1\66\1\137\1\172\1\104\1\uffff\1\101\1\131\1\105\1\uffff"
        u"\3\172\1\115\2\172\1\124\1\uffff\1\131\1\107\1\122\1\120\1\106"
        u"\1\105\1\120\1\122\1\107\1\124\2\uffff\1\62\1\64\1\101\1\124\1"
        u"\uffff\1\105\1\116\1\120\1\122\1\124\3\uffff\1\105\2\uffff\1\131"
        u"\1\120\1\105\1\113\1\105\1\101\1\122\1\105\1\103\1\172\1\131\2"
        u"\172\1\125\1\131\2\172\1\105\1\172\1\131\1\116\1\120\1\105\1\172"
        u"\1\115\1\172\1\125\1\123\1\172\1\105\1\uffff\1\120\2\uffff\1\124"
        u"\1\120\1\124\2\uffff\1\172\1\124\1\uffff\1\120\1\124\1\105\1\172"
        u"\1\124\1\uffff\1\105\1\uffff\1\114\1\111\1\uffff\1\172\1\105\1"
        u"\117\1\105\1\131\1\uffff\1\131\1\105\1\101\1\172\1\uffff\1\131"
        u"\1\123\1\124\1\117\1\124\1\uffff\3\172\2\120\1\172\1\124\1\uffff"
        u"\1\120\1\123\1\172\1\116\1\131\3\uffff\2\105\1\uffff\1\111\1\105"
        u"\1\101\1\uffff\1\172\1\120\2\172\1\117\1\172\1\107\1\uffff\1\105"
        u"\2\uffff\1\116\1\uffff\1\105\3\172\2\uffff\1\124\1\uffff\1\131"
        u"\1\120\1\105\1\172\1\uffff"
        )

    DFA21_accept = DFA.unpack(
        u"\1\uffff\1\1\1\uffff\1\3\16\uffff\1\56\1\uffff\1\61\1\62\1\63\1"
        u"\64\1\65\1\66\1\67\1\70\1\71\1\uffff\1\73\1\74\1\75\1\2\1\60\32"
        u"\uffff\1\72\1\57\22\uffff\1\37\3\uffff\1\43\13\uffff\1\12\1\uffff"
        u"\1\16\11\uffff\1\31\16\uffff\1\10\3\uffff\1\17\7\uffff\1\27\12"
        u"\uffff\1\50\1\52\4\uffff\1\4\5\uffff\1\20\1\22\1\23\1\uffff\1\25"
        u"\1\26\36\uffff\1\47\1\uffff\1\53\1\54\3\uffff\1\6\1\11\2\uffff"
        u"\1\14\5\uffff\1\33\1\uffff\1\40\2\uffff\1\44\5\uffff\1\13\4\uffff"
        u"\1\32\5\uffff\1\45\7\uffff\1\30\5\uffff\1\51\1\55\1\5\2\uffff\1"
        u"\21\3\uffff\1\41\7\uffff\1\42\1\uffff\1\7\1\15\1\uffff\1\34\4\uffff"
        u"\1\46\1\24\1\uffff\1\35\4\uffff\1\36"
        )

    DFA21_special = DFA.unpack(
        u"\u011c\uffff"
        )


    DFA21_transition = [
        DFA.unpack(u"\2\37\2\uffff\1\37\22\uffff\1\37\1\uffff\1\40\1\24\4"
        u"\uffff\1\31\1\32\2\uffff\1\1\1\uffff\1\2\1\36\1\23\11\35\1\3\1"
        u"\25\1\uffff\1\26\3\uffff\1\4\1\5\1\6\1\22\1\7\1\10\2\22\1\11\3"
        u"\22\1\12\1\13\1\14\2\22\1\15\1\16\1\17\1\20\1\22\1\21\3\22\1\33"
        u"\1\uffff\1\34\1\uffff\1\22\1\uffff\32\22\1\27\1\uffff\1\30"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\41\1\uffff\12\42"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\43\3\uffff\1\44\4\uffff\1\45"),
        DFA.unpack(u"\1\46"),
        DFA.unpack(u"\1\47\1\50"),
        DFA.unpack(u"\1\51\7\uffff\1\52"),
        DFA.unpack(u"\1\53\12\uffff\1\54"),
        DFA.unpack(u"\1\55\1\56\1\uffff\1\57\2\uffff\1\60"),
        DFA.unpack(u"\1\61"),
        DFA.unpack(u"\1\62\7\uffff\1\63\1\uffff\1\64"),
        DFA.unpack(u"\1\65\11\uffff\1\66"),
        DFA.unpack(u"\1\67"),
        DFA.unpack(u"\1\70"),
        DFA.unpack(u"\1\71\20\uffff\1\72"),
        DFA.unpack(u"\1\73"),
        DFA.unpack(u"\1\74"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\42\1\uffff\12\35\13\uffff\1\42\22\uffff\1\75\14"
        u"\uffff\1\42\22\uffff\1\75"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\42\1\uffff\12\35\13\uffff\1\42\37\uffff\1\42"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\77"),
        DFA.unpack(u"\1\100"),
        DFA.unpack(u"\1\101"),
        DFA.unpack(u"\1\102"),
        DFA.unpack(u"\1\103\7\uffff\1\104"),
        DFA.unpack(u"\1\105"),
        DFA.unpack(u"\1\106"),
        DFA.unpack(u"\1\107"),
        DFA.unpack(u"\1\110"),
        DFA.unpack(u"\1\111"),
        DFA.unpack(u"\1\112"),
        DFA.unpack(u"\1\113"),
        DFA.unpack(u"\1\114"),
        DFA.unpack(u"\1\115"),
        DFA.unpack(u"\1\116"),
        DFA.unpack(u"\1\117"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\120\1\uffff\32\22"),
        DFA.unpack(u"\1\122"),
        DFA.unpack(u"\1\123"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\124\1\uffff\32\22"),
        DFA.unpack(u"\1\126"),
        DFA.unpack(u"\1\127"),
        DFA.unpack(u"\1\130"),
        DFA.unpack(u"\1\131"),
        DFA.unpack(u"\1\132"),
        DFA.unpack(u"\1\133"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\134"),
        DFA.unpack(u"\1\135"),
        DFA.unpack(u"\1\136"),
        DFA.unpack(u"\1\137"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\140\1\uffff\32\22"),
        DFA.unpack(u"\1\142"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\144"),
        DFA.unpack(u"\1\145"),
        DFA.unpack(u"\1\146"),
        DFA.unpack(u"\1\147"),
        DFA.unpack(u"\1\150"),
        DFA.unpack(u"\1\151\2\uffff\1\152"),
        DFA.unpack(u"\1\153"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\154\1\uffff\32\22"),
        DFA.unpack(u"\1\156"),
        DFA.unpack(u"\1\157"),
        DFA.unpack(u"\1\160"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\161"),
        DFA.unpack(u"\1\162"),
        DFA.unpack(u"\1\163"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\164"),
        DFA.unpack(u"\1\165"),
        DFA.unpack(u"\1\166"),
        DFA.unpack(u"\1\167"),
        DFA.unpack(u"\1\170"),
        DFA.unpack(u"\1\171"),
        DFA.unpack(u"\1\172"),
        DFA.unpack(u"\1\173"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\175"),
        DFA.unpack(u"\1\176"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\177"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u0081"),
        DFA.unpack(u"\1\u0082"),
        DFA.unpack(u"\1\u0083"),
        DFA.unpack(u"\1\u0084"),
        DFA.unpack(u"\1\u0085"),
        DFA.unpack(u"\1\u0086"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u0087\1\uffff\32\22"),
        DFA.unpack(u"\1\u0089"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u008a"),
        DFA.unpack(u"\1\u008b"),
        DFA.unpack(u"\1\u008c"),
        DFA.unpack(u"\1\u008d"),
        DFA.unpack(u"\1\u008e"),
        DFA.unpack(u"\1\u008f"),
        DFA.unpack(u"\1\u0090"),
        DFA.unpack(u"\1\u0091"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u0092\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u0095\2\uffff\1\u0096"),
        DFA.unpack(u"\1\u0097"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u0098\1\uffff\32\22"),
        DFA.unpack(u"\1\u009a"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u009b"),
        DFA.unpack(u"\1\u009c"),
        DFA.unpack(u"\1\u009d"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u009e\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00a2"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00a5"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00a6"),
        DFA.unpack(u"\1\u00a7"),
        DFA.unpack(u"\1\u00a8"),
        DFA.unpack(u"\1\u00a9"),
        DFA.unpack(u"\1\u00aa"),
        DFA.unpack(u"\1\u00ab"),
        DFA.unpack(u"\1\u00ac"),
        DFA.unpack(u"\1\u00ad"),
        DFA.unpack(u"\1\u00ae"),
        DFA.unpack(u"\1\u00af"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00b0"),
        DFA.unpack(u"\1\u00b1"),
        DFA.unpack(u"\1\u00b2"),
        DFA.unpack(u"\1\u00b3"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00b4"),
        DFA.unpack(u"\1\u00b5"),
        DFA.unpack(u"\1\u00b6"),
        DFA.unpack(u"\1\u00b7"),
        DFA.unpack(u"\1\u00b8"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00b9"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00ba"),
        DFA.unpack(u"\1\u00bb"),
        DFA.unpack(u"\1\u00bc"),
        DFA.unpack(u"\1\u00bd"),
        DFA.unpack(u"\1\u00be"),
        DFA.unpack(u"\1\u00bf"),
        DFA.unpack(u"\1\u00c0"),
        DFA.unpack(u"\1\u00c1"),
        DFA.unpack(u"\1\u00c2"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00c4"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00c7"),
        DFA.unpack(u"\1\u00c8"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u00c9\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00cc"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u00cd\1\uffff\32\22"),
        DFA.unpack(u"\1\u00cf"),
        DFA.unpack(u"\1\u00d0"),
        DFA.unpack(u"\1\u00d1"),
        DFA.unpack(u"\1\u00d2"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u00d3\1\uffff\32\22"),
        DFA.unpack(u"\1\u00d5"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00d7"),
        DFA.unpack(u"\1\u00d8"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00da"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00db"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00dc"),
        DFA.unpack(u"\1\u00dd"),
        DFA.unpack(u"\1\u00de"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00e0"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00e1"),
        DFA.unpack(u"\1\u00e2"),
        DFA.unpack(u"\1\u00e3"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00e5"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00e6"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00e7"),
        DFA.unpack(u"\1\u00e8"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u00e9\1\uffff\32\22"),
        DFA.unpack(u"\1\u00eb"),
        DFA.unpack(u"\1\u00ec"),
        DFA.unpack(u"\1\u00ed"),
        DFA.unpack(u"\1\u00ee"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00ef"),
        DFA.unpack(u"\1\u00f0"),
        DFA.unpack(u"\1\u00f1"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00f3"),
        DFA.unpack(u"\1\u00f4"),
        DFA.unpack(u"\1\u00f5"),
        DFA.unpack(u"\1\u00f6"),
        DFA.unpack(u"\1\u00f7"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00fb"),
        DFA.unpack(u"\1\u00fc"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u00fe"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00ff"),
        DFA.unpack(u"\1\u0100"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u0102"),
        DFA.unpack(u"\1\u0103"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0104"),
        DFA.unpack(u"\1\u0105"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0106"),
        DFA.unpack(u"\1\u0107"),
        DFA.unpack(u"\1\u0108"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u010a"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u010d"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\1\u010f"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0110"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0111"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0112"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\u0115\1\uffff\32\22"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0117"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0118"),
        DFA.unpack(u"\1\u0119"),
        DFA.unpack(u"\1\u011a"),
        DFA.unpack(u"\12\22\7\uffff\32\22\4\uffff\1\22\1\uffff\32\22"),
        DFA.unpack(u"")
    ]

    # class definition for DFA #21

    class DFA21(DFA):
        pass


 



def main(argv, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
    from antlr3.main import LexerMain
    main = LexerMain(osekoilLexer)

    main.stdin = stdin
    main.stdout = stdout
    main.stderr = stderr
    main.execute(argv)



if __name__ == '__main__':
    main(sys.argv)
