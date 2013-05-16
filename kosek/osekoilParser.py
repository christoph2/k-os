# $ANTLR 3.4 osekoil.g 2013-05-12 09:27:23

import sys
from antlr3 import *
from antlr3.compat import set, frozenset

from antlr3.tree import *


                
from kosek.BaseParser import BaseParser
import kosek.ApplicationDefinition as ApplicationDefinition
import kosek.ImplementationDefinition as ImplementationDefinition
from kosek.ImplementationDefinition.Extras import Extras



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

# token names
tokenNames = [
    "<invalid>", "<EOR>", "<DOWN>", "<UP>",
    "APPLICATION_DEFINITION", "ASSIGN", "ATTRIBUTE_NAME", "ATTRIBUTE_VALUE", 
    "AUTO_SPECIFIER", "AUTO_VALUE", "BOOLEAN", "BOOL_VALUE", "BOOL_VALUES", 
    "COMMENT", "DECDIGITS", "DEFAULT_BOOL", "DEFAULT_FLOAT", "DEFAULT_NAME", 
    "DEFAULT_NUMBER", "DEFAULT_STRING", "DESCRIPTION", "ENUMERATION", "ENUMERATOR", 
    "ENUMERATOR_LIST", "ESC_SEQ", "EXPONENT", "FILE", "FLOAT", "FLOAT_DEF", 
    "FLOAT_RANGE", "FLOAT_VALUE", "HEX", "HEX_DIGIT", "ID", "ID_VALUE", 
    "IMPLEMENTATION_DEF", "IMPLEMENTATION_DEFINITION", "IMPLEMENTATION_LIST", 
    "IMPLEMENTATION_SPEC", "IMPLEMENTATION_SPEC_LIST", "IMPL_ATTR_DEF", 
    "IMPL_DEF_LIST", "IMPL_PARAMETER_LIST", "IMPL_REF_DEF", "INT", "LBRACK", 
    "LCURLY", "LINE_NUMBER", "LPAREN", "MULTIPLE_SPECIFIER", "NUMBER", "NUMBER_LIST", 
    "NUMBER_RANGE", "NUMBER_VALUE", "OBJECT", "OBJECT_DEFINITION", "OBJECT_DEFINITION_LIST", 
    "OBJECT_NAME", "OBJECT_REF_TYPE", "OCTAL_ESC", "OIL_VERSION", "PARAMETER", 
    "PARAMETER_LIST", "POSDIGIT", "RBRACK", "RCURLY", "REFERENCE_NAME", 
    "RRAREN", "SEMI", "SHIT", "SIGN", "STRING", "STRING_VALUE", "UNICODE_ESC", 
    "WS", "ZERODIGIT", "','", "'..'", "':'", "'ALARM'", "'ALARM_TYPE'", 
    "'APPMODE'", "'APPMODE_TYPE'", "'AUTO'", "'BOOLEAN'", "'COM'", "'COM_TYPE'", 
    "'COUNTER'", "'COUNTER_TYPE'", "'CPU'", "'ENUM'", "'EVENT'", "'EVENT_TYPE'", 
    "'FALSE'", "'FLOAT'", "'IMPLEMENTATION'", "'INT32'", "'INT64'", "'IPDU'", 
    "'IPDU_TYPE'", "'ISR'", "'ISR_TYPE'", "'MESSAGE'", "'MESSAGE_TYPE'", 
    "'NETWORKMESSAGE'", "'NETWORKMESSAGE_TYPE'", "'NM'", "'NM_TYPE'", "'NO_DEFAULT'", 
    "'OIL_VERSION'", "'OS'", "'OS_TYPE'", "'RESOURCE'", "'RESOURCE_TYPE'", 
    "'STRING'", "'TASK'", "'TASK_TYPE'", "'TRUE'", "'UINT32'", "'UINT64'", 
    "'WITH_AUTO'"
]



class implementationSpec_scope(object):
    def __init__(self):
        self.objectType = None


class implAttrDef_scope(object):
    def __init__(self):
        self.implAttrName = None


class objectDefinition_scope(object):
    def __init__(self):
        self.objectName = None


class parameter_scope(object):
    def __init__(self):
        self.attrName = None



class osekoilParser(BaseParser):
    grammarFileName = "osekoil.g"
    api_version = 1
    tokenNames = tokenNames

    def __init__(self, input, state=None, *args, **kwargs):
        if state is None:
            state = RecognizerSharedState()

        super(osekoilParser, self).__init__(input, state, *args, **kwargs)


	self.implementationSpec_stack = []


	self.implAttrDef_stack = []


	self.objectDefinition_stack = []


	self.parameter_stack = []




        self.delegates = []

	self._adaptor = None
	self.adaptor = CommonTreeAdaptor()



    def getTreeAdaptor(self):
        return self._adaptor

    def setTreeAdaptor(self, adaptor):
        self._adaptor = adaptor

    adaptor = property(getTreeAdaptor, setTreeAdaptor)

                     
    implDefinition = dict()
    standardResources = []
    internalResources = []
    linkedResources = []



    class file_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.file_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "file"
    # osekoil.g:112:1: file returns [value] : oilVersion implementationDefinition applicationDefinition -> ^( FILE oilVersion implementationDefinition applicationDefinition ) ;
    def file(self, ):
        retval = self.file_return()
        retval.start = self.input.LT(1)


        root_0 = None

        oilVersion1 = None

        implementationDefinition2 = None

        applicationDefinition3 = None


        stream_oilVersion = RewriteRuleSubtreeStream(self._adaptor, "rule oilVersion")
        stream_implementationDefinition = RewriteRuleSubtreeStream(self._adaptor, "rule implementationDefinition")
        stream_applicationDefinition = RewriteRuleSubtreeStream(self._adaptor, "rule applicationDefinition")
        try:
            try:
                # osekoil.g:113:4: ( oilVersion implementationDefinition applicationDefinition -> ^( FILE oilVersion implementationDefinition applicationDefinition ) )
                # osekoil.g:113:6: oilVersion implementationDefinition applicationDefinition
                pass 
                self._state.following.append(self.FOLLOW_oilVersion_in_file387)
                oilVersion1 = self.oilVersion()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_oilVersion.add(oilVersion1.tree)


                self._state.following.append(self.FOLLOW_implementationDefinition_in_file389)
                implementationDefinition2 = self.implementationDefinition()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_implementationDefinition.add(implementationDefinition2.tree)


                self._state.following.append(self.FOLLOW_applicationDefinition_in_file391)
                applicationDefinition3 = self.applicationDefinition()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_applicationDefinition.add(applicationDefinition3.tree)


                if self._state.backtracking == 0:
                    pass
                       
                    retval.value = (((implementationDefinition2 is not None) and [implementationDefinition2.value] or [None])[0], ((applicationDefinition3 is not None) and [applicationDefinition3.value] or [None])[0])
                       



                # AST Rewrite
                # elements: implementationDefinition, applicationDefinition, oilVersion
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 117:4: -> ^( FILE oilVersion implementationDefinition applicationDefinition )
                    # osekoil.g:117:7: ^( FILE oilVersion implementationDefinition applicationDefinition )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(FILE, "FILE")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_oilVersion.nextTree())

                    self._adaptor.addChild(root_1, stream_implementationDefinition.nextTree())

                    self._adaptor.addChild(root_1, stream_applicationDefinition.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "file"


    class oilVersion_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.oilVersion_return, self).__init__()

            self.tree = None





    # $ANTLR start "oilVersion"
    # osekoil.g:121:1: oilVersion : 'OIL_VERSION' ASSIGN STRING ( description )? SEMI -> ^( OIL_VERSION STRING ( description )? ) ;
    def oilVersion(self, ):
        retval = self.oilVersion_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal4 = None
        ASSIGN5 = None
        STRING6 = None
        SEMI8 = None
        description7 = None


        string_literal4_tree = None
        ASSIGN5_tree = None
        STRING6_tree = None
        SEMI8_tree = None
        stream_109 = RewriteRuleTokenStream(self._adaptor, "token 109")
        stream_SEMI = RewriteRuleTokenStream(self._adaptor, "token SEMI")
        stream_STRING = RewriteRuleTokenStream(self._adaptor, "token STRING")
        stream_ASSIGN = RewriteRuleTokenStream(self._adaptor, "token ASSIGN")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
        try:
            try:
                # osekoil.g:122:4: ( 'OIL_VERSION' ASSIGN STRING ( description )? SEMI -> ^( OIL_VERSION STRING ( description )? ) )
                # osekoil.g:122:6: 'OIL_VERSION' ASSIGN STRING ( description )? SEMI
                pass 
                string_literal4 = self.match(self.input, 109, self.FOLLOW_109_in_oilVersion427) 
                if self._state.backtracking == 0:
                    stream_109.add(string_literal4)


                ASSIGN5 = self.match(self.input, ASSIGN, self.FOLLOW_ASSIGN_in_oilVersion429) 
                if self._state.backtracking == 0:
                    stream_ASSIGN.add(ASSIGN5)


                STRING6 = self.match(self.input, STRING, self.FOLLOW_STRING_in_oilVersion431) 
                if self._state.backtracking == 0:
                    stream_STRING.add(STRING6)


                # osekoil.g:122:34: ( description )?
                alt1 = 2
                LA1_0 = self.input.LA(1)

                if (LA1_0 == 78) :
                    alt1 = 1
                if alt1 == 1:
                    # osekoil.g:122:34: description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_oilVersion433)
                    description7 = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(description7.tree)





                SEMI8 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_oilVersion436) 
                if self._state.backtracking == 0:
                    stream_SEMI.add(SEMI8)


                # AST Rewrite
                # elements: STRING, description
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 123:4: -> ^( OIL_VERSION STRING ( description )? )
                    # osekoil.g:123:7: ^( OIL_VERSION STRING ( description )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(OIL_VERSION, "OIL_VERSION")
                    , root_1)

                    self._adaptor.addChild(root_1, 
                    stream_STRING.nextNode()
                    )

                    # osekoil.g:123:28: ( description )?
                    if stream_description.hasNext():
                        self._adaptor.addChild(root_1, stream_description.nextTree())


                    stream_description.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "oilVersion"


    class implementationDefinition_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implementationDefinition_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "implementationDefinition"
    # osekoil.g:131:1: implementationDefinition returns [value] : 'IMPLEMENTATION' ID '{' implementationSpecList '}' ( description )? SEMI -> ^( IMPLEMENTATION_DEFINITION implementationSpecList ( description )? ) ;
    def implementationDefinition(self, ):
        retval = self.implementationDefinition_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal9 = None
        ID10 = None
        char_literal11 = None
        char_literal13 = None
        SEMI15 = None
        implementationSpecList12 = None

        description14 = None


        string_literal9_tree = None
        ID10_tree = None
        char_literal11_tree = None
        char_literal13_tree = None
        SEMI15_tree = None
        stream_LCURLY = RewriteRuleTokenStream(self._adaptor, "token LCURLY")
        stream_95 = RewriteRuleTokenStream(self._adaptor, "token 95")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_SEMI = RewriteRuleTokenStream(self._adaptor, "token SEMI")
        stream_RCURLY = RewriteRuleTokenStream(self._adaptor, "token RCURLY")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
        stream_implementationSpecList = RewriteRuleSubtreeStream(self._adaptor, "rule implementationSpecList")
        try:
            try:
                # osekoil.g:132:4: ( 'IMPLEMENTATION' ID '{' implementationSpecList '}' ( description )? SEMI -> ^( IMPLEMENTATION_DEFINITION implementationSpecList ( description )? ) )
                # osekoil.g:132:7: 'IMPLEMENTATION' ID '{' implementationSpecList '}' ( description )? SEMI
                pass 
                string_literal9 = self.match(self.input, 95, self.FOLLOW_95_in_implementationDefinition474) 
                if self._state.backtracking == 0:
                    stream_95.add(string_literal9)


                ID10 = self.match(self.input, ID, self.FOLLOW_ID_in_implementationDefinition476) 
                if self._state.backtracking == 0:
                    stream_ID.add(ID10)


                char_literal11 = self.match(self.input, LCURLY, self.FOLLOW_LCURLY_in_implementationDefinition478) 
                if self._state.backtracking == 0:
                    stream_LCURLY.add(char_literal11)


                self._state.following.append(self.FOLLOW_implementationSpecList_in_implementationDefinition480)
                implementationSpecList12 = self.implementationSpecList()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_implementationSpecList.add(implementationSpecList12.tree)


                char_literal13 = self.match(self.input, RCURLY, self.FOLLOW_RCURLY_in_implementationDefinition482) 
                if self._state.backtracking == 0:
                    stream_RCURLY.add(char_literal13)


                # osekoil.g:132:58: ( description )?
                alt2 = 2
                LA2_0 = self.input.LA(1)

                if (LA2_0 == 78) :
                    alt2 = 1
                if alt2 == 1:
                    # osekoil.g:132:58: description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_implementationDefinition484)
                    description14 = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(description14.tree)





                SEMI15 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implementationDefinition487) 
                if self._state.backtracking == 0:
                    stream_SEMI.add(SEMI15)


                if self._state.backtracking == 0:
                    pass
                       
                    retval.value = (((implementationSpecList12 is not None) and [implementationSpecList12.value] or [None])[0], ((description14 is not None) and [description14.value] or [None])[0])
                    self.implDefinition = ((implementationSpecList12 is not None) and [implementationSpecList12.value] or [None])[0]
                       



                # AST Rewrite
                # elements: description, implementationSpecList
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 137:4: -> ^( IMPLEMENTATION_DEFINITION implementationSpecList ( description )? )
                    # osekoil.g:137:7: ^( IMPLEMENTATION_DEFINITION implementationSpecList ( description )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IMPLEMENTATION_DEFINITION, "IMPLEMENTATION_DEFINITION")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_implementationSpecList.nextTree())

                    # osekoil.g:137:58: ( description )?
                    if stream_description.hasNext():
                        self._adaptor.addChild(root_1, stream_description.nextTree())


                    stream_description.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "implementationDefinition"


    class implementationSpecList_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implementationSpecList_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "implementationSpecList"
    # osekoil.g:141:1: implementationSpecList returns [value] : (specs+= ( implementationSpec ) )* -> ^( IMPLEMENTATION_SPEC_LIST ( implementationSpec )* ) ;
    def implementationSpecList(self, ):
        retval = self.implementationSpecList_return()
        retval.start = self.input.LT(1)


        root_0 = None

        specs = None
        list_specs = None
        implementationSpec16 = None


        specs_tree = None
        stream_implementationSpec = RewriteRuleSubtreeStream(self._adaptor, "rule implementationSpec")
              
        myList = []

        try:
            try:
                # osekoil.g:149:4: ( (specs+= ( implementationSpec ) )* -> ^( IMPLEMENTATION_SPEC_LIST ( implementationSpec )* ) )
                # osekoil.g:149:7: (specs+= ( implementationSpec ) )*
                pass 
                # osekoil.g:149:13: (specs+= ( implementationSpec ) )*
                while True: #loop3
                    alt3 = 2
                    LA3_0 = self.input.LA(1)

                    if (LA3_0 == 79 or LA3_0 == 81 or LA3_0 == 85 or LA3_0 == 87 or LA3_0 == 91 or LA3_0 == 98 or LA3_0 == 100 or LA3_0 == 102 or LA3_0 == 104 or LA3_0 == 106 or LA3_0 == 110 or LA3_0 == 112 or LA3_0 == 115) :
                        alt3 = 1


                    if alt3 == 1:
                        # osekoil.g:149:13: specs+= ( implementationSpec )
                        pass 
                        # osekoil.g:149:16: ( implementationSpec )
                        # osekoil.g:149:17: implementationSpec
                        pass 
                        self._state.following.append(self.FOLLOW_implementationSpec_in_implementationSpecList541)
                        implementationSpec16 = self.implementationSpec()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_implementationSpec.add(implementationSpec16.tree)


                        if self._state.backtracking == 0:
                            pass
                            myList.append(((implementationSpec16 is not None) and [implementationSpec16.value] or [None])[0]) 







                    else:
                        break #loop3


                # AST Rewrite
                # elements: implementationSpec
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 150:4: -> ^( IMPLEMENTATION_SPEC_LIST ( implementationSpec )* )
                    # osekoil.g:150:7: ^( IMPLEMENTATION_SPEC_LIST ( implementationSpec )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IMPLEMENTATION_SPEC_LIST, "IMPLEMENTATION_SPEC_LIST")
                    , root_1)

                    # osekoil.g:150:34: ( implementationSpec )*
                    while stream_implementationSpec.hasNext():
                        self._adaptor.addChild(root_1, stream_implementationSpec.nextTree())


                    stream_implementationSpec.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



                if self._state.backtracking == 0:
                    pass
                           
                    builder = ImplementationDefinition.ImplementationDefinitionBuilder(self, myList)
                    retval.value = builder.postProcessing()




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "implementationSpecList"


    class implementationSpec_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implementationSpec_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "implementationSpec"
    # osekoil.g:153:1: implementationSpec returns [value] : object '{' implementationList '}' ( description )? SEMI -> ^( IMPLEMENTATION_SPEC object implementationList ( description )? ) ;
    def implementationSpec(self, ):
        self.implementationSpec_stack.append(implementationSpec_scope())
        retval = self.implementationSpec_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal18 = None
        char_literal20 = None
        SEMI22 = None
        object17 = None

        implementationList19 = None

        description21 = None


        char_literal18_tree = None
        char_literal20_tree = None
        SEMI22_tree = None
        stream_LCURLY = RewriteRuleTokenStream(self._adaptor, "token LCURLY")
        stream_SEMI = RewriteRuleTokenStream(self._adaptor, "token SEMI")
        stream_RCURLY = RewriteRuleTokenStream(self._adaptor, "token RCURLY")
        stream_implementationList = RewriteRuleSubtreeStream(self._adaptor, "rule implementationList")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
        stream_object = RewriteRuleSubtreeStream(self._adaptor, "rule object")
              
        self.implementationSpec_stack[-1].objectType = []

        try:
            try:
                # osekoil.g:160:4: ( object '{' implementationList '}' ( description )? SEMI -> ^( IMPLEMENTATION_SPEC object implementationList ( description )? ) )
                # osekoil.g:160:7: object '{' implementationList '}' ( description )? SEMI
                pass 
                self._state.following.append(self.FOLLOW_object_in_implementationSpec587)
                object17 = self.object()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_object.add(object17.tree)


                if self._state.backtracking == 0:
                    pass
                    self.implementationSpec_stack[-1].objectType = ((object17 is not None) and [self.input.toString(object17.start,object17.stop)] or [None])[0] 



                char_literal18 = self.match(self.input, LCURLY, self.FOLLOW_LCURLY_in_implementationSpec591) 
                if self._state.backtracking == 0:
                    stream_LCURLY.add(char_literal18)


                self._state.following.append(self.FOLLOW_implementationList_in_implementationSpec593)
                implementationList19 = self.implementationList()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_implementationList.add(implementationList19.tree)


                char_literal20 = self.match(self.input, RCURLY, self.FOLLOW_RCURLY_in_implementationSpec595) 
                if self._state.backtracking == 0:
                    stream_RCURLY.add(char_literal20)


                # osekoil.g:160:92: ( description )?
                alt4 = 2
                LA4_0 = self.input.LA(1)

                if (LA4_0 == 78) :
                    alt4 = 1
                if alt4 == 1:
                    # osekoil.g:160:92: description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_implementationSpec597)
                    description21 = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(description21.tree)





                SEMI22 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implementationSpec600) 
                if self._state.backtracking == 0:
                    stream_SEMI.add(SEMI22)


                if self._state.backtracking == 0:
                    pass
                       
                    retval.value = ImplementationDefinition.ImplementationSpec(self.implementationSpec_stack[-1].objectType, ((implementationList19 is not None) and [implementationList19.value] or [None])[0], ((description21 is not None) and [description21.value] or [None])[0])
                       



                # AST Rewrite
                # elements: implementationList, object, description
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 164:7: -> ^( IMPLEMENTATION_SPEC object implementationList ( description )? )
                    # osekoil.g:164:10: ^( IMPLEMENTATION_SPEC object implementationList ( description )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IMPLEMENTATION_SPEC, "IMPLEMENTATION_SPEC")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_object.nextTree())

                    self._adaptor.addChild(root_1, stream_implementationList.nextTree())

                    # osekoil.g:164:58: ( description )?
                    if stream_description.hasNext():
                        self._adaptor.addChild(root_1, stream_description.nextTree())


                    stream_description.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            self.implementationSpec_stack.pop()
            pass
        return retval

    # $ANTLR end "implementationSpec"


    class object_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.object_return, self).__init__()

            self.tree = None





    # $ANTLR start "object"
    # osekoil.g:167:1: object : (id= 'OS' |id= 'TASK' |id= 'COUNTER' |id= 'ALARM' |id= 'RESOURCE' |id= 'EVENT' |id= 'ISR' |id= 'MESSAGE' |id= 'COM' |id= 'NM' |id= 'APPMODE' |id= 'IPDU' |id= 'NETWORKMESSAGE' -> ^( OBJECT $id) );
    def object(self, ):
        retval = self.object_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None

        id_tree = None
        stream_104 = RewriteRuleTokenStream(self._adaptor, "token 104")

        try:
            try:
                # osekoil.g:168:4: (id= 'OS' |id= 'TASK' |id= 'COUNTER' |id= 'ALARM' |id= 'RESOURCE' |id= 'EVENT' |id= 'ISR' |id= 'MESSAGE' |id= 'COM' |id= 'NM' |id= 'APPMODE' |id= 'IPDU' |id= 'NETWORKMESSAGE' -> ^( OBJECT $id) )
                alt5 = 13
                LA5 = self.input.LA(1)
                if LA5 == 110:
                    alt5 = 1
                elif LA5 == 115:
                    alt5 = 2
                elif LA5 == 87:
                    alt5 = 3
                elif LA5 == 79:
                    alt5 = 4
                elif LA5 == 112:
                    alt5 = 5
                elif LA5 == 91:
                    alt5 = 6
                elif LA5 == 100:
                    alt5 = 7
                elif LA5 == 102:
                    alt5 = 8
                elif LA5 == 85:
                    alt5 = 9
                elif LA5 == 106:
                    alt5 = 10
                elif LA5 == 81:
                    alt5 = 11
                elif LA5 == 98:
                    alt5 = 12
                elif LA5 == 104:
                    alt5 = 13
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 5, 0, self.input)

                    raise nvae


                if alt5 == 1:
                    # osekoil.g:168:7: id= 'OS'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 110, self.FOLLOW_110_in_object644)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 2:
                    # osekoil.g:169:7: id= 'TASK'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 115, self.FOLLOW_115_in_object656)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 3:
                    # osekoil.g:170:7: id= 'COUNTER'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 87, self.FOLLOW_87_in_object668)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 4:
                    # osekoil.g:171:7: id= 'ALARM'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 79, self.FOLLOW_79_in_object680)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 5:
                    # osekoil.g:172:7: id= 'RESOURCE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 112, self.FOLLOW_112_in_object692)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 6:
                    # osekoil.g:173:7: id= 'EVENT'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 91, self.FOLLOW_91_in_object704)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 7:
                    # osekoil.g:174:7: id= 'ISR'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 100, self.FOLLOW_100_in_object716)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 8:
                    # osekoil.g:175:7: id= 'MESSAGE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 102, self.FOLLOW_102_in_object728)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 9:
                    # osekoil.g:176:7: id= 'COM'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 85, self.FOLLOW_85_in_object740)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 10:
                    # osekoil.g:177:7: id= 'NM'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 106, self.FOLLOW_106_in_object752)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 11:
                    # osekoil.g:178:7: id= 'APPMODE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 81, self.FOLLOW_81_in_object764)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 12:
                    # osekoil.g:179:7: id= 'IPDU'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 98, self.FOLLOW_98_in_object776)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt5 == 13:
                    # osekoil.g:180:7: id= 'NETWORKMESSAGE'
                    pass 
                    id = self.match(self.input, 104, self.FOLLOW_104_in_object788) 
                    if self._state.backtracking == 0:
                        stream_104.add(id)


                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 181:4: -> ^( OBJECT $id)
                        # osekoil.g:181:7: ^( OBJECT $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(OBJECT, "OBJECT")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "object"


    class implementationList_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implementationList_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "implementationList"
    # osekoil.g:184:1: implementationList returns [value] : (ids+= ( implementationDef ) )* -> ^( IMPLEMENTATION_LIST ( implementationDef )* ) ;
    def implementationList(self, ):
        retval = self.implementationList_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ids = None
        list_ids = None
        implementationDef23 = None


        ids_tree = None
        stream_implementationDef = RewriteRuleSubtreeStream(self._adaptor, "rule implementationDef")
              
        myList = []

        try:
            try:
                # osekoil.g:191:4: ( (ids+= ( implementationDef ) )* -> ^( IMPLEMENTATION_LIST ( implementationDef )* ) )
                # osekoil.g:191:7: (ids+= ( implementationDef ) )*
                pass 
                # osekoil.g:191:11: (ids+= ( implementationDef ) )*
                while True: #loop6
                    alt6 = 2
                    LA6_0 = self.input.LA(1)

                    if (LA6_0 == 80 or LA6_0 == 82 or LA6_0 == 84 or LA6_0 == 86 or LA6_0 == 88 or LA6_0 == 90 or LA6_0 == 92 or LA6_0 == 94 or (96 <= LA6_0 <= 97) or LA6_0 == 99 or LA6_0 == 101 or LA6_0 == 103 or LA6_0 == 105 or LA6_0 == 107 or LA6_0 == 111 or (113 <= LA6_0 <= 114) or LA6_0 == 116 or (118 <= LA6_0 <= 119)) :
                        alt6 = 1


                    if alt6 == 1:
                        # osekoil.g:191:11: ids+= ( implementationDef )
                        pass 
                        # osekoil.g:191:14: ( implementationDef )
                        # osekoil.g:191:15: implementationDef
                        pass 
                        self._state.following.append(self.FOLLOW_implementationDef_in_implementationList834)
                        implementationDef23 = self.implementationDef()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_implementationDef.add(implementationDef23.tree)


                        if self._state.backtracking == 0:
                            pass
                            myList.append(((implementationDef23 is not None) and [implementationDef23.values] or [None])[0]) 







                    else:
                        break #loop6


                # AST Rewrite
                # elements: implementationDef
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 192:4: -> ^( IMPLEMENTATION_LIST ( implementationDef )* )
                    # osekoil.g:192:7: ^( IMPLEMENTATION_LIST ( implementationDef )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IMPLEMENTATION_LIST, "IMPLEMENTATION_LIST")
                    , root_1)

                    # osekoil.g:192:29: ( implementationDef )*
                    while stream_implementationDef.hasNext():
                        self._adaptor.addChild(root_1, stream_implementationDef.nextTree())


                    stream_implementationDef.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



                if self._state.backtracking == 0:
                    pass
                           
                    retval.value = myList




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "implementationList"


    class implementationDef_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implementationDef_return, self).__init__()

            self.values = None
            self.tree = None





    # $ANTLR start "implementationDef"
    # osekoil.g:195:1: implementationDef returns [values] : (id= implAttrDef -> ^( IMPLEMENTATION_DEF implAttrDef ) |id2= implRefDef -> ^( IMPLEMENTATION_DEF implRefDef ) );
    def implementationDef(self, ):
        retval = self.implementationDef_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None

        id2 = None


        stream_implRefDef = RewriteRuleSubtreeStream(self._adaptor, "rule implRefDef")
        stream_implAttrDef = RewriteRuleSubtreeStream(self._adaptor, "rule implAttrDef")
        try:
            try:
                # osekoil.g:199:4: (id= implAttrDef -> ^( IMPLEMENTATION_DEF implAttrDef ) |id2= implRefDef -> ^( IMPLEMENTATION_DEF implRefDef ) )
                alt7 = 2
                LA7_0 = self.input.LA(1)

                if (LA7_0 == 84 or LA7_0 == 90 or LA7_0 == 94 or (96 <= LA7_0 <= 97) or LA7_0 == 114 or (118 <= LA7_0 <= 119)) :
                    alt7 = 1
                elif (LA7_0 == 80 or LA7_0 == 82 or LA7_0 == 86 or LA7_0 == 88 or LA7_0 == 92 or LA7_0 == 99 or LA7_0 == 101 or LA7_0 == 103 or LA7_0 == 105 or LA7_0 == 107 or LA7_0 == 111 or LA7_0 == 113 or LA7_0 == 116) :
                    alt7 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 7, 0, self.input)

                    raise nvae


                if alt7 == 1:
                    # osekoil.g:199:7: id= implAttrDef
                    pass 
                    self._state.following.append(self.FOLLOW_implAttrDef_in_implementationDef879)
                    id = self.implAttrDef()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_implAttrDef.add(id.tree)


                    if self._state.backtracking == 0:
                        pass
                           
                        retval.values = ImplementationDefinition.ImplAttrDef(((id is not None) and [id.dataType] or [None])[0], ((id is not None) and [id.attrName] or [None])[0], ((id is not None) and [id.auto] or [None])[0], ((id is not None) and [id.mult] or [None])[0], ((id is not None) and [id.deflt] or [None])[0], ((id is not None) and [id.desc] or [None])[0], ((id is not None) and [id.extra] or [None])[0])
                           



                    # AST Rewrite
                    # elements: implAttrDef
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 203:4: -> ^( IMPLEMENTATION_DEF implAttrDef )
                        # osekoil.g:203:7: ^( IMPLEMENTATION_DEF implAttrDef )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPLEMENTATION_DEF, "IMPLEMENTATION_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_implAttrDef.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt7 == 2:
                    # osekoil.g:204:6: id2= implRefDef
                    pass 
                    self._state.following.append(self.FOLLOW_implRefDef_in_implementationDef906)
                    id2 = self.implRefDef()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_implRefDef.add(id2.tree)


                    if self._state.backtracking == 0:
                        pass
                           
                        retval.values = ImplementationDefinition.ImplRefDef(((id2 is not None) and [id2.objectType] or [None])[0], ((id2 is not None) and [id2.name] or [None])[0], ((id2 is not None) and [id2.mult] or [None])[0], ((id2 is not None) and [id2.desc] or [None])[0])
                           



                    # AST Rewrite
                    # elements: implRefDef
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 208:4: -> ^( IMPLEMENTATION_DEF implRefDef )
                        # osekoil.g:208:7: ^( IMPLEMENTATION_DEF implRefDef )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPLEMENTATION_DEF, "IMPLEMENTATION_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_implRefDef.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


                if self._state.backtracking == 0:
                    pass
                           
                    ###




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "implementationDef"


    class implAttrDef_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implAttrDef_return, self).__init__()

            self.dataType = None
            self.attrName = None
            self.auto = None
            self.mult = None
            self.deflt = None
            self.desc = None
            self.extra = None
            self.tree = None





    # $ANTLR start "implAttrDef"
    # osekoil.g:211:1: implAttrDef returns [dataType, attrName, auto, mult, deflt, desc, extra] : ( 'UINT32' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'UINT32' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? ) | 'INT32' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'INT32' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? ) | 'UINT64' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'UINT64' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? ) | 'INT64' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'INT64' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? ) | 'FLOAT' ( autoSpecifier )? ( floatRange )? attributeName ( multipleSpecifier )? ( defaultFloat )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'FLOAT' attributeName ( autoSpecifier )? ( floatRange )? ( multipleSpecifier )? ( defaultFloat )? ( description )? ) | 'ENUM' ( autoSpecifier )? enumeration attributeName ( multipleSpecifier )? ( defaultName )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'ENUM' attributeName ( autoSpecifier )? enumeration ( multipleSpecifier )? ( defaultName )? ( description )? ) | 'STRING' ( autoSpecifier )? attributeName ( multipleSpecifier )? ( defaultString )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'STRING' attributeName ( autoSpecifier )? ( multipleSpecifier )? ( defaultString )? ( description )? ) | 'BOOLEAN' ( autoSpecifier )? ( boolValues )? attributeName ( multipleSpecifier )? ( defaultBool )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'BOOLEAN' attributeName ( autoSpecifier )? ( boolValues )? ( multipleSpecifier )? ( defaultBool )? ( description )? ) );
    def implAttrDef(self, ):
        self.implAttrDef_stack.append(implAttrDef_scope())
        retval = self.implAttrDef_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal24 = None
        SEMI31 = None
        string_literal32 = None
        SEMI39 = None
        string_literal40 = None
        SEMI47 = None
        string_literal48 = None
        SEMI55 = None
        string_literal56 = None
        SEMI63 = None
        string_literal64 = None
        SEMI71 = None
        string_literal72 = None
        SEMI78 = None
        string_literal79 = None
        SEMI86 = None
        autoSpecifier25 = None

        numberRange26 = None

        attributeName27 = None

        multipleSpecifier28 = None

        defaultNumber29 = None

        description30 = None

        autoSpecifier33 = None

        numberRange34 = None

        attributeName35 = None

        multipleSpecifier36 = None

        defaultNumber37 = None

        description38 = None

        autoSpecifier41 = None

        numberRange42 = None

        attributeName43 = None

        multipleSpecifier44 = None

        defaultNumber45 = None

        description46 = None

        autoSpecifier49 = None

        numberRange50 = None

        attributeName51 = None

        multipleSpecifier52 = None

        defaultNumber53 = None

        description54 = None

        autoSpecifier57 = None

        floatRange58 = None

        attributeName59 = None

        multipleSpecifier60 = None

        defaultFloat61 = None

        description62 = None

        autoSpecifier65 = None

        enumeration66 = None

        attributeName67 = None

        multipleSpecifier68 = None

        defaultName69 = None

        description70 = None

        autoSpecifier73 = None

        attributeName74 = None

        multipleSpecifier75 = None

        defaultString76 = None

        description77 = None

        autoSpecifier80 = None

        boolValues81 = None

        attributeName82 = None

        multipleSpecifier83 = None

        defaultBool84 = None

        description85 = None


        string_literal24_tree = None
        SEMI31_tree = None
        string_literal32_tree = None
        SEMI39_tree = None
        string_literal40_tree = None
        SEMI47_tree = None
        string_literal48_tree = None
        SEMI55_tree = None
        string_literal56_tree = None
        SEMI63_tree = None
        string_literal64_tree = None
        SEMI71_tree = None
        string_literal72_tree = None
        SEMI78_tree = None
        string_literal79_tree = None
        SEMI86_tree = None
        stream_97 = RewriteRuleTokenStream(self._adaptor, "token 97")
        stream_114 = RewriteRuleTokenStream(self._adaptor, "token 114")
        stream_96 = RewriteRuleTokenStream(self._adaptor, "token 96")
        stream_94 = RewriteRuleTokenStream(self._adaptor, "token 94")
        stream_90 = RewriteRuleTokenStream(self._adaptor, "token 90")
        stream_SEMI = RewriteRuleTokenStream(self._adaptor, "token SEMI")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_118 = RewriteRuleTokenStream(self._adaptor, "token 118")
        stream_119 = RewriteRuleTokenStream(self._adaptor, "token 119")
        stream_boolValues = RewriteRuleSubtreeStream(self._adaptor, "rule boolValues")
        stream_defaultName = RewriteRuleSubtreeStream(self._adaptor, "rule defaultName")
        stream_attributeName = RewriteRuleSubtreeStream(self._adaptor, "rule attributeName")
        stream_floatRange = RewriteRuleSubtreeStream(self._adaptor, "rule floatRange")
        stream_numberRange = RewriteRuleSubtreeStream(self._adaptor, "rule numberRange")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
        stream_defaultNumber = RewriteRuleSubtreeStream(self._adaptor, "rule defaultNumber")
        stream_enumeration = RewriteRuleSubtreeStream(self._adaptor, "rule enumeration")
        stream_defaultFloat = RewriteRuleSubtreeStream(self._adaptor, "rule defaultFloat")
        stream_autoSpecifier = RewriteRuleSubtreeStream(self._adaptor, "rule autoSpecifier")
        stream_multipleSpecifier = RewriteRuleSubtreeStream(self._adaptor, "rule multipleSpecifier")
        stream_defaultString = RewriteRuleSubtreeStream(self._adaptor, "rule defaultString")
        stream_defaultBool = RewriteRuleSubtreeStream(self._adaptor, "rule defaultBool")
              


        try:
            try:
                # osekoil.g:226:4: ( 'UINT32' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'UINT32' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? ) | 'INT32' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'INT32' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? ) | 'UINT64' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'UINT64' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? ) | 'INT64' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'INT64' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? ) | 'FLOAT' ( autoSpecifier )? ( floatRange )? attributeName ( multipleSpecifier )? ( defaultFloat )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'FLOAT' attributeName ( autoSpecifier )? ( floatRange )? ( multipleSpecifier )? ( defaultFloat )? ( description )? ) | 'ENUM' ( autoSpecifier )? enumeration attributeName ( multipleSpecifier )? ( defaultName )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'ENUM' attributeName ( autoSpecifier )? enumeration ( multipleSpecifier )? ( defaultName )? ( description )? ) | 'STRING' ( autoSpecifier )? attributeName ( multipleSpecifier )? ( defaultString )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'STRING' attributeName ( autoSpecifier )? ( multipleSpecifier )? ( defaultString )? ( description )? ) | 'BOOLEAN' ( autoSpecifier )? ( boolValues )? attributeName ( multipleSpecifier )? ( defaultBool )? ( description )? SEMI -> ^( IMPL_ATTR_DEF 'BOOLEAN' attributeName ( autoSpecifier )? ( boolValues )? ( multipleSpecifier )? ( defaultBool )? ( description )? ) )
                alt46 = 8
                LA46 = self.input.LA(1)
                if LA46 == 118:
                    alt46 = 1
                elif LA46 == 96:
                    alt46 = 2
                elif LA46 == 119:
                    alt46 = 3
                elif LA46 == 97:
                    alt46 = 4
                elif LA46 == 94:
                    alt46 = 5
                elif LA46 == 90:
                    alt46 = 6
                elif LA46 == 114:
                    alt46 = 7
                elif LA46 == 84:
                    alt46 = 8
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 46, 0, self.input)

                    raise nvae


                if alt46 == 1:
                    # osekoil.g:226:7: 'UINT32' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI
                    pass 
                    string_literal24 = self.match(self.input, 118, self.FOLLOW_118_in_implAttrDef957) 
                    if self._state.backtracking == 0:
                        stream_118.add(string_literal24)


                    # osekoil.g:226:16: ( autoSpecifier )?
                    alt8 = 2
                    LA8_0 = self.input.LA(1)

                    if (LA8_0 == 120) :
                        alt8 = 1
                    if alt8 == 1:
                        # osekoil.g:226:16: autoSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_autoSpecifier_in_implAttrDef959)
                        autoSpecifier25 = self.autoSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_autoSpecifier.add(autoSpecifier25.tree)





                    # osekoil.g:226:31: ( numberRange )?
                    alt9 = 2
                    LA9_0 = self.input.LA(1)

                    if (LA9_0 == LBRACK) :
                        alt9 = 1
                    if alt9 == 1:
                        # osekoil.g:226:31: numberRange
                        pass 
                        self._state.following.append(self.FOLLOW_numberRange_in_implAttrDef962)
                        numberRange26 = self.numberRange()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_numberRange.add(numberRange26.tree)





                    self._state.following.append(self.FOLLOW_attributeName_in_implAttrDef965)
                    attributeName27 = self.attributeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_attributeName.add(attributeName27.tree)


                    # osekoil.g:226:58: ( multipleSpecifier )?
                    alt10 = 2
                    LA10_0 = self.input.LA(1)

                    if (LA10_0 == LBRACK) :
                        alt10 = 1
                    if alt10 == 1:
                        # osekoil.g:226:58: multipleSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_multipleSpecifier_in_implAttrDef967)
                        multipleSpecifier28 = self.multipleSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_multipleSpecifier.add(multipleSpecifier28.tree)





                    # osekoil.g:226:77: ( defaultNumber )?
                    alt11 = 2
                    LA11_0 = self.input.LA(1)

                    if (LA11_0 == ASSIGN) :
                        alt11 = 1
                    if alt11 == 1:
                        # osekoil.g:226:77: defaultNumber
                        pass 
                        self._state.following.append(self.FOLLOW_defaultNumber_in_implAttrDef970)
                        defaultNumber29 = self.defaultNumber()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_defaultNumber.add(defaultNumber29.tree)





                    # osekoil.g:226:92: ( description )?
                    alt12 = 2
                    LA12_0 = self.input.LA(1)

                    if (LA12_0 == 78) :
                        alt12 = 1
                    if alt12 == 1:
                        # osekoil.g:226:92: description
                        pass 
                        self._state.following.append(self.FOLLOW_description_in_implAttrDef973)
                        description30 = self.description()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_description.add(description30.tree)





                    SEMI31 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implAttrDef976) 
                    if self._state.backtracking == 0:
                        stream_SEMI.add(SEMI31)


                    if self._state.backtracking == 0:
                        pass
                              
                        retval.dataType = ImplementationDefinition.ImplAttrType.UINT32
                        retval.attrName = ((attributeName27 is not None) and [self.input.toString(attributeName27.start,attributeName27.stop)] or [None])[0]

                        self.implAttrDef_stack[-1].implAttrName = ((attributeName27 is not None) and [self.input.toString(attributeName27.start,attributeName27.stop)] or [None])[0]


                        retval.extra = Extras(Extras.NUMBER_RANGE, {'range' : ((numberRange26 is not None) and [numberRange26.range] or [None])[0], 'numberFrom': ((numberRange26 is not None) and [numberRange26.nfrom] or [None])[0],
                            'numberTo': ((numberRange26 is not None) and [numberRange26.nto] or [None])[0]})
                        if ((autoSpecifier25 is not None) and [self.input.toString(autoSpecifier25.start,autoSpecifier25.stop)] or [None])[0]: retval.auto = True
                        else: retval.auto = False
                        if ((multipleSpecifier28 is not None) and [self.input.toString(multipleSpecifier28.start,multipleSpecifier28.stop)] or [None])[0]: retval.mult = True
                        else: retval.mult = False
                        retval.deflt = ((defaultNumber29 is not None) and [defaultNumber29.value] or [None])[0]
                        if ((description30 is not None) and [description30.value] or [None])[0]: retval.desc = ((description30 is not None) and [description30.value] or [None])[0]
                        else: retval.desc = ""
                              



                    # AST Rewrite
                    # elements: defaultNumber, autoSpecifier, 118, description, multipleSpecifier, attributeName, numberRange
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 244:7: -> ^( IMPL_ATTR_DEF 'UINT32' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? )
                        # osekoil.g:244:10: ^( IMPL_ATTR_DEF 'UINT32' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPL_ATTR_DEF, "IMPL_ATTR_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_118.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                        # osekoil.g:244:49: ( autoSpecifier )?
                        if stream_autoSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_autoSpecifier.nextTree())


                        stream_autoSpecifier.reset();

                        # osekoil.g:244:64: ( numberRange )?
                        if stream_numberRange.hasNext():
                            self._adaptor.addChild(root_1, stream_numberRange.nextTree())


                        stream_numberRange.reset();

                        # osekoil.g:244:77: ( multipleSpecifier )?
                        if stream_multipleSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                        stream_multipleSpecifier.reset();

                        # osekoil.g:244:96: ( defaultNumber )?
                        if stream_defaultNumber.hasNext():
                            self._adaptor.addChild(root_1, stream_defaultNumber.nextTree())


                        stream_defaultNumber.reset();

                        # osekoil.g:244:111: ( description )?
                        if stream_description.hasNext():
                            self._adaptor.addChild(root_1, stream_description.nextTree())


                        stream_description.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt46 == 2:
                    # osekoil.g:245:7: 'INT32' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI
                    pass 
                    string_literal32 = self.match(self.input, 96, self.FOLLOW_96_in_implAttrDef1023) 
                    if self._state.backtracking == 0:
                        stream_96.add(string_literal32)


                    # osekoil.g:245:19: ( autoSpecifier )?
                    alt13 = 2
                    LA13_0 = self.input.LA(1)

                    if (LA13_0 == 120) :
                        alt13 = 1
                    if alt13 == 1:
                        # osekoil.g:245:19: autoSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_autoSpecifier_in_implAttrDef1029)
                        autoSpecifier33 = self.autoSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_autoSpecifier.add(autoSpecifier33.tree)





                    # osekoil.g:245:34: ( numberRange )?
                    alt14 = 2
                    LA14_0 = self.input.LA(1)

                    if (LA14_0 == LBRACK) :
                        alt14 = 1
                    if alt14 == 1:
                        # osekoil.g:245:34: numberRange
                        pass 
                        self._state.following.append(self.FOLLOW_numberRange_in_implAttrDef1032)
                        numberRange34 = self.numberRange()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_numberRange.add(numberRange34.tree)





                    self._state.following.append(self.FOLLOW_attributeName_in_implAttrDef1035)
                    attributeName35 = self.attributeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_attributeName.add(attributeName35.tree)


                    # osekoil.g:245:61: ( multipleSpecifier )?
                    alt15 = 2
                    LA15_0 = self.input.LA(1)

                    if (LA15_0 == LBRACK) :
                        alt15 = 1
                    if alt15 == 1:
                        # osekoil.g:245:61: multipleSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_multipleSpecifier_in_implAttrDef1037)
                        multipleSpecifier36 = self.multipleSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_multipleSpecifier.add(multipleSpecifier36.tree)





                    # osekoil.g:245:80: ( defaultNumber )?
                    alt16 = 2
                    LA16_0 = self.input.LA(1)

                    if (LA16_0 == ASSIGN) :
                        alt16 = 1
                    if alt16 == 1:
                        # osekoil.g:245:80: defaultNumber
                        pass 
                        self._state.following.append(self.FOLLOW_defaultNumber_in_implAttrDef1040)
                        defaultNumber37 = self.defaultNumber()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_defaultNumber.add(defaultNumber37.tree)





                    # osekoil.g:245:95: ( description )?
                    alt17 = 2
                    LA17_0 = self.input.LA(1)

                    if (LA17_0 == 78) :
                        alt17 = 1
                    if alt17 == 1:
                        # osekoil.g:245:95: description
                        pass 
                        self._state.following.append(self.FOLLOW_description_in_implAttrDef1043)
                        description38 = self.description()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_description.add(description38.tree)





                    SEMI39 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implAttrDef1046) 
                    if self._state.backtracking == 0:
                        stream_SEMI.add(SEMI39)


                    if self._state.backtracking == 0:
                        pass
                              
                        retval.dataType = ImplementationDefinition.ImplAttrType.INT32
                        retval.attrName = ((attributeName35 is not None) and [self.input.toString(attributeName35.start,attributeName35.stop)] or [None])[0]

                        self.implAttrDef_stack[-1].implAttrName = ((attributeName35 is not None) and [self.input.toString(attributeName35.start,attributeName35.stop)] or [None])[0]

                        retval.extra = Extras(Extras.NUMBER_RANGE, {'range' : ((numberRange34 is not None) and [numberRange34.range] or [None])[0], 'numberFrom': ((numberRange34 is not None) and [numberRange34.nfrom] or [None])[0],
                            'numberTo': ((numberRange34 is not None) and [numberRange34.nto] or [None])[0]})
                        if ((autoSpecifier33 is not None) and [self.input.toString(autoSpecifier33.start,autoSpecifier33.stop)] or [None])[0]: retval.auto = True
                        else: retval.auto = False
                        if ((multipleSpecifier36 is not None) and [self.input.toString(multipleSpecifier36.start,multipleSpecifier36.stop)] or [None])[0]: retval.mult = True
                        else: retval.mult = False
                        retval.deflt = ((defaultNumber37 is not None) and [defaultNumber37.value] or [None])[0]
                        if ((description38 is not None) and [description38.value] or [None])[0]: retval.desc = ((description38 is not None) and [description38.value] or [None])[0]
                        else: retval.desc = ""
                              



                    # AST Rewrite
                    # elements: defaultNumber, attributeName, numberRange, multipleSpecifier, description, 96, autoSpecifier
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 262:7: -> ^( IMPL_ATTR_DEF 'INT32' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? )
                        # osekoil.g:262:10: ^( IMPL_ATTR_DEF 'INT32' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPL_ATTR_DEF, "IMPL_ATTR_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_96.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                        # osekoil.g:262:48: ( autoSpecifier )?
                        if stream_autoSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_autoSpecifier.nextTree())


                        stream_autoSpecifier.reset();

                        # osekoil.g:262:63: ( numberRange )?
                        if stream_numberRange.hasNext():
                            self._adaptor.addChild(root_1, stream_numberRange.nextTree())


                        stream_numberRange.reset();

                        # osekoil.g:262:76: ( multipleSpecifier )?
                        if stream_multipleSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                        stream_multipleSpecifier.reset();

                        # osekoil.g:262:95: ( defaultNumber )?
                        if stream_defaultNumber.hasNext():
                            self._adaptor.addChild(root_1, stream_defaultNumber.nextTree())


                        stream_defaultNumber.reset();

                        # osekoil.g:262:110: ( description )?
                        if stream_description.hasNext():
                            self._adaptor.addChild(root_1, stream_description.nextTree())


                        stream_description.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt46 == 3:
                    # osekoil.g:263:7: 'UINT64' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI
                    pass 
                    string_literal40 = self.match(self.input, 119, self.FOLLOW_119_in_implAttrDef1093) 
                    if self._state.backtracking == 0:
                        stream_119.add(string_literal40)


                    # osekoil.g:263:16: ( autoSpecifier )?
                    alt18 = 2
                    LA18_0 = self.input.LA(1)

                    if (LA18_0 == 120) :
                        alt18 = 1
                    if alt18 == 1:
                        # osekoil.g:263:16: autoSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_autoSpecifier_in_implAttrDef1095)
                        autoSpecifier41 = self.autoSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_autoSpecifier.add(autoSpecifier41.tree)





                    # osekoil.g:263:31: ( numberRange )?
                    alt19 = 2
                    LA19_0 = self.input.LA(1)

                    if (LA19_0 == LBRACK) :
                        alt19 = 1
                    if alt19 == 1:
                        # osekoil.g:263:31: numberRange
                        pass 
                        self._state.following.append(self.FOLLOW_numberRange_in_implAttrDef1098)
                        numberRange42 = self.numberRange()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_numberRange.add(numberRange42.tree)





                    self._state.following.append(self.FOLLOW_attributeName_in_implAttrDef1101)
                    attributeName43 = self.attributeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_attributeName.add(attributeName43.tree)


                    # osekoil.g:263:58: ( multipleSpecifier )?
                    alt20 = 2
                    LA20_0 = self.input.LA(1)

                    if (LA20_0 == LBRACK) :
                        alt20 = 1
                    if alt20 == 1:
                        # osekoil.g:263:58: multipleSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_multipleSpecifier_in_implAttrDef1103)
                        multipleSpecifier44 = self.multipleSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_multipleSpecifier.add(multipleSpecifier44.tree)





                    # osekoil.g:263:77: ( defaultNumber )?
                    alt21 = 2
                    LA21_0 = self.input.LA(1)

                    if (LA21_0 == ASSIGN) :
                        alt21 = 1
                    if alt21 == 1:
                        # osekoil.g:263:77: defaultNumber
                        pass 
                        self._state.following.append(self.FOLLOW_defaultNumber_in_implAttrDef1106)
                        defaultNumber45 = self.defaultNumber()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_defaultNumber.add(defaultNumber45.tree)





                    # osekoil.g:263:92: ( description )?
                    alt22 = 2
                    LA22_0 = self.input.LA(1)

                    if (LA22_0 == 78) :
                        alt22 = 1
                    if alt22 == 1:
                        # osekoil.g:263:92: description
                        pass 
                        self._state.following.append(self.FOLLOW_description_in_implAttrDef1109)
                        description46 = self.description()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_description.add(description46.tree)





                    SEMI47 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implAttrDef1112) 
                    if self._state.backtracking == 0:
                        stream_SEMI.add(SEMI47)


                    if self._state.backtracking == 0:
                        pass
                              
                        retval.dataType = ImplementationDefinition.ImplAttrType.UINT64
                        retval.attrName = ((attributeName43 is not None) and [self.input.toString(attributeName43.start,attributeName43.stop)] or [None])[0]

                        self.implAttrDef_stack[-1].implAttrName = ((attributeName43 is not None) and [self.input.toString(attributeName43.start,attributeName43.stop)] or [None])[0]

                        retval.extra = Extras(Extras.NUMBER_RANGE, {'range' : ((numberRange42 is not None) and [numberRange42.range] or [None])[0], 'numberFrom': ((numberRange42 is not None) and [numberRange42.nfrom] or [None])[0],
                            'numberTo': ((numberRange42 is not None) and [numberRange42.nto] or [None])[0]})
                        if ((autoSpecifier41 is not None) and [self.input.toString(autoSpecifier41.start,autoSpecifier41.stop)] or [None])[0]: retval.auto = True
                        else: retval.auto = False
                        if ((multipleSpecifier44 is not None) and [self.input.toString(multipleSpecifier44.start,multipleSpecifier44.stop)] or [None])[0]: retval.mult = True
                        else: retval.mult = False
                        retval.deflt = ((defaultNumber45 is not None) and [defaultNumber45.value] or [None])[0]
                        if ((description46 is not None) and [description46.value] or [None])[0]: retval.desc = ((description46 is not None) and [description46.value] or [None])[0]
                        else: retval.desc = ""
                              



                    # AST Rewrite
                    # elements: description, 119, defaultNumber, autoSpecifier, multipleSpecifier, numberRange, attributeName
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 280:7: -> ^( IMPL_ATTR_DEF 'UINT64' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? )
                        # osekoil.g:280:10: ^( IMPL_ATTR_DEF 'UINT64' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPL_ATTR_DEF, "IMPL_ATTR_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_119.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                        # osekoil.g:280:49: ( autoSpecifier )?
                        if stream_autoSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_autoSpecifier.nextTree())


                        stream_autoSpecifier.reset();

                        # osekoil.g:280:64: ( numberRange )?
                        if stream_numberRange.hasNext():
                            self._adaptor.addChild(root_1, stream_numberRange.nextTree())


                        stream_numberRange.reset();

                        # osekoil.g:280:77: ( multipleSpecifier )?
                        if stream_multipleSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                        stream_multipleSpecifier.reset();

                        # osekoil.g:280:96: ( defaultNumber )?
                        if stream_defaultNumber.hasNext():
                            self._adaptor.addChild(root_1, stream_defaultNumber.nextTree())


                        stream_defaultNumber.reset();

                        # osekoil.g:280:111: ( description )?
                        if stream_description.hasNext():
                            self._adaptor.addChild(root_1, stream_description.nextTree())


                        stream_description.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt46 == 4:
                    # osekoil.g:281:7: 'INT64' ( autoSpecifier )? ( numberRange )? attributeName ( multipleSpecifier )? ( defaultNumber )? ( description )? SEMI
                    pass 
                    string_literal48 = self.match(self.input, 97, self.FOLLOW_97_in_implAttrDef1159) 
                    if self._state.backtracking == 0:
                        stream_97.add(string_literal48)


                    # osekoil.g:281:19: ( autoSpecifier )?
                    alt23 = 2
                    LA23_0 = self.input.LA(1)

                    if (LA23_0 == 120) :
                        alt23 = 1
                    if alt23 == 1:
                        # osekoil.g:281:19: autoSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_autoSpecifier_in_implAttrDef1165)
                        autoSpecifier49 = self.autoSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_autoSpecifier.add(autoSpecifier49.tree)





                    # osekoil.g:281:34: ( numberRange )?
                    alt24 = 2
                    LA24_0 = self.input.LA(1)

                    if (LA24_0 == LBRACK) :
                        alt24 = 1
                    if alt24 == 1:
                        # osekoil.g:281:34: numberRange
                        pass 
                        self._state.following.append(self.FOLLOW_numberRange_in_implAttrDef1168)
                        numberRange50 = self.numberRange()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_numberRange.add(numberRange50.tree)





                    self._state.following.append(self.FOLLOW_attributeName_in_implAttrDef1171)
                    attributeName51 = self.attributeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_attributeName.add(attributeName51.tree)


                    # osekoil.g:281:61: ( multipleSpecifier )?
                    alt25 = 2
                    LA25_0 = self.input.LA(1)

                    if (LA25_0 == LBRACK) :
                        alt25 = 1
                    if alt25 == 1:
                        # osekoil.g:281:61: multipleSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_multipleSpecifier_in_implAttrDef1173)
                        multipleSpecifier52 = self.multipleSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_multipleSpecifier.add(multipleSpecifier52.tree)





                    # osekoil.g:281:80: ( defaultNumber )?
                    alt26 = 2
                    LA26_0 = self.input.LA(1)

                    if (LA26_0 == ASSIGN) :
                        alt26 = 1
                    if alt26 == 1:
                        # osekoil.g:281:80: defaultNumber
                        pass 
                        self._state.following.append(self.FOLLOW_defaultNumber_in_implAttrDef1176)
                        defaultNumber53 = self.defaultNumber()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_defaultNumber.add(defaultNumber53.tree)





                    # osekoil.g:281:95: ( description )?
                    alt27 = 2
                    LA27_0 = self.input.LA(1)

                    if (LA27_0 == 78) :
                        alt27 = 1
                    if alt27 == 1:
                        # osekoil.g:281:95: description
                        pass 
                        self._state.following.append(self.FOLLOW_description_in_implAttrDef1179)
                        description54 = self.description()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_description.add(description54.tree)





                    SEMI55 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implAttrDef1182) 
                    if self._state.backtracking == 0:
                        stream_SEMI.add(SEMI55)


                    if self._state.backtracking == 0:
                        pass
                              
                        retval.dataType = ImplementationDefinition.ImplAttrType.INT64
                        retval.attrName = ((attributeName51 is not None) and [self.input.toString(attributeName51.start,attributeName51.stop)] or [None])[0]
                        retval.extra = Extras(Extras.NUMBER_RANGE, {'range' : ((numberRange50 is not None) and [numberRange50.range] or [None])[0], 'numberFrom': ((numberRange50 is not None) and [numberRange50.nfrom] or [None])[0],
                            'numberTo': ((numberRange50 is not None) and [numberRange50.nto] or [None])[0]})

                        self.implAttrDef_stack[-1].implAttrName = ((attributeName51 is not None) and [self.input.toString(attributeName51.start,attributeName51.stop)] or [None])[0]

                        if ((autoSpecifier49 is not None) and [self.input.toString(autoSpecifier49.start,autoSpecifier49.stop)] or [None])[0]: retval.auto = True
                        else: retval.auto = False
                        if ((multipleSpecifier52 is not None) and [self.input.toString(multipleSpecifier52.start,multipleSpecifier52.stop)] or [None])[0]: retval.mult = True
                        else: retval.mult = False
                        retval.deflt = ((defaultNumber53 is not None) and [defaultNumber53.value] or [None])[0]
                        if ((description54 is not None) and [description54.value] or [None])[0]: retval.desc = ((description54 is not None) and [description54.value] or [None])[0]
                        else: retval.desc = ""
                              



                    # AST Rewrite
                    # elements: autoSpecifier, 97, description, defaultNumber, multipleSpecifier, attributeName, numberRange
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 298:7: -> ^( IMPL_ATTR_DEF 'INT64' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? )
                        # osekoil.g:298:10: ^( IMPL_ATTR_DEF 'INT64' attributeName ( autoSpecifier )? ( numberRange )? ( multipleSpecifier )? ( defaultNumber )? ( description )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPL_ATTR_DEF, "IMPL_ATTR_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_97.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                        # osekoil.g:298:48: ( autoSpecifier )?
                        if stream_autoSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_autoSpecifier.nextTree())


                        stream_autoSpecifier.reset();

                        # osekoil.g:298:63: ( numberRange )?
                        if stream_numberRange.hasNext():
                            self._adaptor.addChild(root_1, stream_numberRange.nextTree())


                        stream_numberRange.reset();

                        # osekoil.g:298:76: ( multipleSpecifier )?
                        if stream_multipleSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                        stream_multipleSpecifier.reset();

                        # osekoil.g:298:95: ( defaultNumber )?
                        if stream_defaultNumber.hasNext():
                            self._adaptor.addChild(root_1, stream_defaultNumber.nextTree())


                        stream_defaultNumber.reset();

                        # osekoil.g:298:110: ( description )?
                        if stream_description.hasNext():
                            self._adaptor.addChild(root_1, stream_description.nextTree())


                        stream_description.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt46 == 5:
                    # osekoil.g:299:7: 'FLOAT' ( autoSpecifier )? ( floatRange )? attributeName ( multipleSpecifier )? ( defaultFloat )? ( description )? SEMI
                    pass 
                    string_literal56 = self.match(self.input, 94, self.FOLLOW_94_in_implAttrDef1229) 
                    if self._state.backtracking == 0:
                        stream_94.add(string_literal56)


                    # osekoil.g:299:19: ( autoSpecifier )?
                    alt28 = 2
                    LA28_0 = self.input.LA(1)

                    if (LA28_0 == 120) :
                        alt28 = 1
                    if alt28 == 1:
                        # osekoil.g:299:19: autoSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_autoSpecifier_in_implAttrDef1235)
                        autoSpecifier57 = self.autoSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_autoSpecifier.add(autoSpecifier57.tree)





                    # osekoil.g:299:34: ( floatRange )?
                    alt29 = 2
                    LA29_0 = self.input.LA(1)

                    if (LA29_0 == LBRACK) :
                        alt29 = 1
                    if alt29 == 1:
                        # osekoil.g:299:34: floatRange
                        pass 
                        self._state.following.append(self.FOLLOW_floatRange_in_implAttrDef1238)
                        floatRange58 = self.floatRange()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_floatRange.add(floatRange58.tree)





                    self._state.following.append(self.FOLLOW_attributeName_in_implAttrDef1241)
                    attributeName59 = self.attributeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_attributeName.add(attributeName59.tree)


                    # osekoil.g:299:60: ( multipleSpecifier )?
                    alt30 = 2
                    LA30_0 = self.input.LA(1)

                    if (LA30_0 == LBRACK) :
                        alt30 = 1
                    if alt30 == 1:
                        # osekoil.g:299:60: multipleSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_multipleSpecifier_in_implAttrDef1243)
                        multipleSpecifier60 = self.multipleSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_multipleSpecifier.add(multipleSpecifier60.tree)





                    # osekoil.g:299:79: ( defaultFloat )?
                    alt31 = 2
                    LA31_0 = self.input.LA(1)

                    if (LA31_0 == ASSIGN) :
                        alt31 = 1
                    if alt31 == 1:
                        # osekoil.g:299:79: defaultFloat
                        pass 
                        self._state.following.append(self.FOLLOW_defaultFloat_in_implAttrDef1246)
                        defaultFloat61 = self.defaultFloat()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_defaultFloat.add(defaultFloat61.tree)





                    # osekoil.g:299:93: ( description )?
                    alt32 = 2
                    LA32_0 = self.input.LA(1)

                    if (LA32_0 == 78) :
                        alt32 = 1
                    if alt32 == 1:
                        # osekoil.g:299:93: description
                        pass 
                        self._state.following.append(self.FOLLOW_description_in_implAttrDef1249)
                        description62 = self.description()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_description.add(description62.tree)





                    SEMI63 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implAttrDef1252) 
                    if self._state.backtracking == 0:
                        stream_SEMI.add(SEMI63)


                    if self._state.backtracking == 0:
                        pass
                              
                        retval.dataType = ImplementationDefinition.ImplAttrType.FLOAT
                        retval.attrName = ((attributeName59 is not None) and [self.input.toString(attributeName59.start,attributeName59.stop)] or [None])[0]
                        retval.extra = Extras(Extras.FLOAT_RANGE, {'numberFrom': ((floatRange58 is not None) and [floatRange58.nfrom] or [None])[0], 'numberTo': ((floatRange58 is not None) and [floatRange58.nto] or [None])[0]})

                        self.implAttrDef_stack[-1].implAttrName = ((attributeName59 is not None) and [self.input.toString(attributeName59.start,attributeName59.stop)] or [None])[0]

                        if ((autoSpecifier57 is not None) and [self.input.toString(autoSpecifier57.start,autoSpecifier57.stop)] or [None])[0]: retval.auto = True
                        else: retval.auto = False
                        if ((multipleSpecifier60 is not None) and [self.input.toString(multipleSpecifier60.start,multipleSpecifier60.stop)] or [None])[0]: retval.mult = True
                        else: retval.mult = False
                        retval.deflt = ((defaultFloat61 is not None) and [defaultFloat61.value] or [None])[0]
                        if ((description62 is not None) and [description62.value] or [None])[0]: retval.desc = ((description62 is not None) and [description62.value] or [None])[0]
                        else: retval.desc = ""
                              



                    # AST Rewrite
                    # elements: 94, defaultFloat, description, multipleSpecifier, floatRange, autoSpecifier, attributeName
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 315:7: -> ^( IMPL_ATTR_DEF 'FLOAT' attributeName ( autoSpecifier )? ( floatRange )? ( multipleSpecifier )? ( defaultFloat )? ( description )? )
                        # osekoil.g:315:10: ^( IMPL_ATTR_DEF 'FLOAT' attributeName ( autoSpecifier )? ( floatRange )? ( multipleSpecifier )? ( defaultFloat )? ( description )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPL_ATTR_DEF, "IMPL_ATTR_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_94.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                        # osekoil.g:315:48: ( autoSpecifier )?
                        if stream_autoSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_autoSpecifier.nextTree())


                        stream_autoSpecifier.reset();

                        # osekoil.g:315:63: ( floatRange )?
                        if stream_floatRange.hasNext():
                            self._adaptor.addChild(root_1, stream_floatRange.nextTree())


                        stream_floatRange.reset();

                        # osekoil.g:315:75: ( multipleSpecifier )?
                        if stream_multipleSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                        stream_multipleSpecifier.reset();

                        # osekoil.g:315:94: ( defaultFloat )?
                        if stream_defaultFloat.hasNext():
                            self._adaptor.addChild(root_1, stream_defaultFloat.nextTree())


                        stream_defaultFloat.reset();

                        # osekoil.g:315:108: ( description )?
                        if stream_description.hasNext():
                            self._adaptor.addChild(root_1, stream_description.nextTree())


                        stream_description.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt46 == 6:
                    # osekoil.g:316:7: 'ENUM' ( autoSpecifier )? enumeration attributeName ( multipleSpecifier )? ( defaultName )? ( description )? SEMI
                    pass 
                    string_literal64 = self.match(self.input, 90, self.FOLLOW_90_in_implAttrDef1299) 
                    if self._state.backtracking == 0:
                        stream_90.add(string_literal64)


                    # osekoil.g:316:19: ( autoSpecifier )?
                    alt33 = 2
                    LA33_0 = self.input.LA(1)

                    if (LA33_0 == 120) :
                        alt33 = 1
                    if alt33 == 1:
                        # osekoil.g:316:19: autoSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_autoSpecifier_in_implAttrDef1306)
                        autoSpecifier65 = self.autoSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_autoSpecifier.add(autoSpecifier65.tree)





                    self._state.following.append(self.FOLLOW_enumeration_in_implAttrDef1309)
                    enumeration66 = self.enumeration()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_enumeration.add(enumeration66.tree)


                    self._state.following.append(self.FOLLOW_attributeName_in_implAttrDef1311)
                    attributeName67 = self.attributeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_attributeName.add(attributeName67.tree)


                    # osekoil.g:316:60: ( multipleSpecifier )?
                    alt34 = 2
                    LA34_0 = self.input.LA(1)

                    if (LA34_0 == LBRACK) :
                        alt34 = 1
                    if alt34 == 1:
                        # osekoil.g:316:60: multipleSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_multipleSpecifier_in_implAttrDef1313)
                        multipleSpecifier68 = self.multipleSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_multipleSpecifier.add(multipleSpecifier68.tree)





                    # osekoil.g:316:79: ( defaultName )?
                    alt35 = 2
                    LA35_0 = self.input.LA(1)

                    if (LA35_0 == ASSIGN) :
                        alt35 = 1
                    if alt35 == 1:
                        # osekoil.g:316:79: defaultName
                        pass 
                        self._state.following.append(self.FOLLOW_defaultName_in_implAttrDef1316)
                        defaultName69 = self.defaultName()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_defaultName.add(defaultName69.tree)





                    # osekoil.g:316:92: ( description )?
                    alt36 = 2
                    LA36_0 = self.input.LA(1)

                    if (LA36_0 == 78) :
                        alt36 = 1
                    if alt36 == 1:
                        # osekoil.g:316:92: description
                        pass 
                        self._state.following.append(self.FOLLOW_description_in_implAttrDef1319)
                        description70 = self.description()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_description.add(description70.tree)





                    SEMI71 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implAttrDef1322) 
                    if self._state.backtracking == 0:
                        stream_SEMI.add(SEMI71)


                    if self._state.backtracking == 0:
                        pass
                              
                        retval.dataType = ImplementationDefinition.ImplAttrType.ENUM
                        retval.attrName = ((attributeName67 is not None) and [self.input.toString(attributeName67.start,attributeName67.stop)] or [None])[0]

                        self.implAttrDef_stack[-1].implAttrName = ((attributeName67 is not None) and [self.input.toString(attributeName67.start,attributeName67.stop)] or [None])[0]

                        retval.extra = Extras(Extras.ENUMERATION, {'enumeration': ((enumeration66 is not None) and [enumeration66.values] or [None])[0]})
                        if ((autoSpecifier65 is not None) and [self.input.toString(autoSpecifier65.start,autoSpecifier65.stop)] or [None])[0]: retval.auto = True
                        else: retval.auto = False
                        if ((multipleSpecifier68 is not None) and [self.input.toString(multipleSpecifier68.start,multipleSpecifier68.stop)] or [None])[0]: retval.mult = True
                        else: retval.mult = False
                        retval.deflt = ((defaultName69 is not None) and [defaultName69.value] or [None])[0]
                        if ((description70 is not None) and [description70.value] or [None])[0]: retval.desc = ((description70 is not None) and [description70.value] or [None])[0]
                        else: retval.desc = ""
                              



                    # AST Rewrite
                    # elements: description, attributeName, enumeration, autoSpecifier, multipleSpecifier, defaultName, 90
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 332:7: -> ^( IMPL_ATTR_DEF 'ENUM' attributeName ( autoSpecifier )? enumeration ( multipleSpecifier )? ( defaultName )? ( description )? )
                        # osekoil.g:332:10: ^( IMPL_ATTR_DEF 'ENUM' attributeName ( autoSpecifier )? enumeration ( multipleSpecifier )? ( defaultName )? ( description )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPL_ATTR_DEF, "IMPL_ATTR_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_90.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                        # osekoil.g:332:47: ( autoSpecifier )?
                        if stream_autoSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_autoSpecifier.nextTree())


                        stream_autoSpecifier.reset();

                        self._adaptor.addChild(root_1, stream_enumeration.nextTree())

                        # osekoil.g:332:74: ( multipleSpecifier )?
                        if stream_multipleSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                        stream_multipleSpecifier.reset();

                        # osekoil.g:332:93: ( defaultName )?
                        if stream_defaultName.hasNext():
                            self._adaptor.addChild(root_1, stream_defaultName.nextTree())


                        stream_defaultName.reset();

                        # osekoil.g:332:106: ( description )?
                        if stream_description.hasNext():
                            self._adaptor.addChild(root_1, stream_description.nextTree())


                        stream_description.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt46 == 7:
                    # osekoil.g:333:7: 'STRING' ( autoSpecifier )? attributeName ( multipleSpecifier )? ( defaultString )? ( description )? SEMI
                    pass 
                    string_literal72 = self.match(self.input, 114, self.FOLLOW_114_in_implAttrDef1368) 
                    if self._state.backtracking == 0:
                        stream_114.add(string_literal72)


                    # osekoil.g:333:16: ( autoSpecifier )?
                    alt37 = 2
                    LA37_0 = self.input.LA(1)

                    if (LA37_0 == 120) :
                        alt37 = 1
                    if alt37 == 1:
                        # osekoil.g:333:16: autoSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_autoSpecifier_in_implAttrDef1370)
                        autoSpecifier73 = self.autoSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_autoSpecifier.add(autoSpecifier73.tree)





                    self._state.following.append(self.FOLLOW_attributeName_in_implAttrDef1373)
                    attributeName74 = self.attributeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_attributeName.add(attributeName74.tree)


                    # osekoil.g:333:45: ( multipleSpecifier )?
                    alt38 = 2
                    LA38_0 = self.input.LA(1)

                    if (LA38_0 == LBRACK) :
                        alt38 = 1
                    if alt38 == 1:
                        # osekoil.g:333:45: multipleSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_multipleSpecifier_in_implAttrDef1375)
                        multipleSpecifier75 = self.multipleSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_multipleSpecifier.add(multipleSpecifier75.tree)





                    # osekoil.g:333:64: ( defaultString )?
                    alt39 = 2
                    LA39_0 = self.input.LA(1)

                    if (LA39_0 == ASSIGN) :
                        alt39 = 1
                    if alt39 == 1:
                        # osekoil.g:333:64: defaultString
                        pass 
                        self._state.following.append(self.FOLLOW_defaultString_in_implAttrDef1378)
                        defaultString76 = self.defaultString()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_defaultString.add(defaultString76.tree)





                    # osekoil.g:333:79: ( description )?
                    alt40 = 2
                    LA40_0 = self.input.LA(1)

                    if (LA40_0 == 78) :
                        alt40 = 1
                    if alt40 == 1:
                        # osekoil.g:333:79: description
                        pass 
                        self._state.following.append(self.FOLLOW_description_in_implAttrDef1381)
                        description77 = self.description()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_description.add(description77.tree)





                    SEMI78 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implAttrDef1384) 
                    if self._state.backtracking == 0:
                        stream_SEMI.add(SEMI78)


                    if self._state.backtracking == 0:
                        pass
                              
                        retval.dataType = ImplementationDefinition.ImplAttrType.STRING
                        retval.attrName = ((attributeName74 is not None) and [self.input.toString(attributeName74.start,attributeName74.stop)] or [None])[0]

                        self.implAttrDef_stack[-1].implAttrName = ((attributeName74 is not None) and [self.input.toString(attributeName74.start,attributeName74.stop)] or [None])[0]
                        retval.extra = Extras(Extras.DUMMY)

                        if ((autoSpecifier73 is not None) and [self.input.toString(autoSpecifier73.start,autoSpecifier73.stop)] or [None])[0]: retval.auto = True
                        else: retval.auto = False
                        if ((multipleSpecifier75 is not None) and [self.input.toString(multipleSpecifier75.start,multipleSpecifier75.stop)] or [None])[0]: retval.mult = True
                        else: retval.mult = False
                        retval.deflt = ((defaultString76 is not None) and [defaultString76.value] or [None])[0]
                        if ((description77 is not None) and [description77.value] or [None])[0]: retval.desc = ((description77 is not None) and [description77.value] or [None])[0]
                        else: retval.desc = ""
                              



                    # AST Rewrite
                    # elements: defaultString, autoSpecifier, 114, multipleSpecifier, attributeName, description
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 349:7: -> ^( IMPL_ATTR_DEF 'STRING' attributeName ( autoSpecifier )? ( multipleSpecifier )? ( defaultString )? ( description )? )
                        # osekoil.g:349:10: ^( IMPL_ATTR_DEF 'STRING' attributeName ( autoSpecifier )? ( multipleSpecifier )? ( defaultString )? ( description )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPL_ATTR_DEF, "IMPL_ATTR_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_114.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                        # osekoil.g:349:49: ( autoSpecifier )?
                        if stream_autoSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_autoSpecifier.nextTree())


                        stream_autoSpecifier.reset();

                        # osekoil.g:349:64: ( multipleSpecifier )?
                        if stream_multipleSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                        stream_multipleSpecifier.reset();

                        # osekoil.g:349:83: ( defaultString )?
                        if stream_defaultString.hasNext():
                            self._adaptor.addChild(root_1, stream_defaultString.nextTree())


                        stream_defaultString.reset();

                        # osekoil.g:349:98: ( description )?
                        if stream_description.hasNext():
                            self._adaptor.addChild(root_1, stream_description.nextTree())


                        stream_description.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt46 == 8:
                    # osekoil.g:350:7: 'BOOLEAN' ( autoSpecifier )? ( boolValues )? attributeName ( multipleSpecifier )? ( defaultBool )? ( description )? SEMI
                    pass 
                    string_literal79 = self.match(self.input, 84, self.FOLLOW_84_in_implAttrDef1428) 
                    if self._state.backtracking == 0:
                        stream_84.add(string_literal79)


                    # osekoil.g:350:19: ( autoSpecifier )?
                    alt41 = 2
                    LA41_0 = self.input.LA(1)

                    if (LA41_0 == 120) :
                        alt41 = 1
                    if alt41 == 1:
                        # osekoil.g:350:19: autoSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_autoSpecifier_in_implAttrDef1432)
                        autoSpecifier80 = self.autoSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_autoSpecifier.add(autoSpecifier80.tree)





                    # osekoil.g:350:34: ( boolValues )?
                    alt42 = 2
                    LA42_0 = self.input.LA(1)

                    if (LA42_0 == LBRACK) :
                        alt42 = 1
                    if alt42 == 1:
                        # osekoil.g:350:34: boolValues
                        pass 
                        self._state.following.append(self.FOLLOW_boolValues_in_implAttrDef1435)
                        boolValues81 = self.boolValues()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_boolValues.add(boolValues81.tree)





                    self._state.following.append(self.FOLLOW_attributeName_in_implAttrDef1438)
                    attributeName82 = self.attributeName()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_attributeName.add(attributeName82.tree)


                    # osekoil.g:350:60: ( multipleSpecifier )?
                    alt43 = 2
                    LA43_0 = self.input.LA(1)

                    if (LA43_0 == LBRACK) :
                        alt43 = 1
                    if alt43 == 1:
                        # osekoil.g:350:60: multipleSpecifier
                        pass 
                        self._state.following.append(self.FOLLOW_multipleSpecifier_in_implAttrDef1440)
                        multipleSpecifier83 = self.multipleSpecifier()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_multipleSpecifier.add(multipleSpecifier83.tree)





                    # osekoil.g:350:79: ( defaultBool )?
                    alt44 = 2
                    LA44_0 = self.input.LA(1)

                    if (LA44_0 == ASSIGN) :
                        alt44 = 1
                    if alt44 == 1:
                        # osekoil.g:350:79: defaultBool
                        pass 
                        self._state.following.append(self.FOLLOW_defaultBool_in_implAttrDef1443)
                        defaultBool84 = self.defaultBool()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_defaultBool.add(defaultBool84.tree)





                    # osekoil.g:350:92: ( description )?
                    alt45 = 2
                    LA45_0 = self.input.LA(1)

                    if (LA45_0 == 78) :
                        alt45 = 1
                    if alt45 == 1:
                        # osekoil.g:350:92: description
                        pass 
                        self._state.following.append(self.FOLLOW_description_in_implAttrDef1446)
                        description85 = self.description()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_description.add(description85.tree)





                    SEMI86 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implAttrDef1449) 
                    if self._state.backtracking == 0:
                        stream_SEMI.add(SEMI86)


                    if self._state.backtracking == 0:
                        pass
                              
                        retval.dataType = ImplementationDefinition.ImplAttrType.BOOLEAN
                        retval.attrName = ((attributeName82 is not None) and [self.input.toString(attributeName82.start,attributeName82.stop)] or [None])[0]

                        self.implAttrDef_stack[-1].implAttrName = ((attributeName82 is not None) and [self.input.toString(attributeName82.start,attributeName82.stop)] or [None])[0]

                        retval.extra = Extras(Extras.BOOL_VALUES, {'boolValues': ((boolValues81 is not None) and [boolValues81.values] or [None])[0]})

                        if ((autoSpecifier80 is not None) and [self.input.toString(autoSpecifier80.start,autoSpecifier80.stop)] or [None])[0]: retval.auto = True
                        else: retval.auto = False
                        if ((multipleSpecifier83 is not None) and [self.input.toString(multipleSpecifier83.start,multipleSpecifier83.stop)] or [None])[0]: retval.mult = True
                        else: retval.mult = False
                        retval.deflt = ((defaultBool84 is not None) and [defaultBool84.value] or [None])[0]
                        if ((description85 is not None) and [description85.value] or [None])[0]: retval.desc = ((description85 is not None) and [description85.value] or [None])[0]
                        else: retval.desc = ""
                              



                    # AST Rewrite
                    # elements: autoSpecifier, boolValues, description, multipleSpecifier, defaultBool, attributeName, 84
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 367:7: -> ^( IMPL_ATTR_DEF 'BOOLEAN' attributeName ( autoSpecifier )? ( boolValues )? ( multipleSpecifier )? ( defaultBool )? ( description )? )
                        # osekoil.g:367:10: ^( IMPL_ATTR_DEF 'BOOLEAN' attributeName ( autoSpecifier )? ( boolValues )? ( multipleSpecifier )? ( defaultBool )? ( description )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(IMPL_ATTR_DEF, "IMPL_ATTR_DEF")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_84.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                        # osekoil.g:367:50: ( autoSpecifier )?
                        if stream_autoSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_autoSpecifier.nextTree())


                        stream_autoSpecifier.reset();

                        # osekoil.g:367:65: ( boolValues )?
                        if stream_boolValues.hasNext():
                            self._adaptor.addChild(root_1, stream_boolValues.nextTree())


                        stream_boolValues.reset();

                        # osekoil.g:367:77: ( multipleSpecifier )?
                        if stream_multipleSpecifier.hasNext():
                            self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                        stream_multipleSpecifier.reset();

                        # osekoil.g:367:96: ( defaultBool )?
                        if stream_defaultBool.hasNext():
                            self._adaptor.addChild(root_1, stream_defaultBool.nextTree())


                        stream_defaultBool.reset();

                        # osekoil.g:367:109: ( description )?
                        if stream_description.hasNext():
                            self._adaptor.addChild(root_1, stream_description.nextTree())


                        stream_description.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



                if self._state.backtracking == 0:
                    pass
                           

                    if retval.deflt:
                       self.appendDefault(self.implementationSpec_stack[-1].objectType, retval.attrName, retval.deflt)
                    if retval.auto:
                       self.appendAutos(self.implementationSpec_stack[-1].objectType, retval.attrName)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            self.implAttrDef_stack.pop()
            pass
        return retval

    # $ANTLR end "implAttrDef"


    class attributeName_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.attributeName_return, self).__init__()

            self.tree = None





    # $ANTLR start "attributeName"
    # osekoil.g:370:1: attributeName : ( ID -> ^( ATTRIBUTE_NAME ID ) | object -> ^( ATTRIBUTE_NAME object ) );
    def attributeName(self, ):
        retval = self.attributeName_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID87 = None
        object88 = None


        ID87_tree = None
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_object = RewriteRuleSubtreeStream(self._adaptor, "rule object")
        try:
            try:
                # osekoil.g:371:4: ( ID -> ^( ATTRIBUTE_NAME ID ) | object -> ^( ATTRIBUTE_NAME object ) )
                alt47 = 2
                LA47_0 = self.input.LA(1)

                if (LA47_0 == ID) :
                    alt47 = 1
                elif (LA47_0 == 79 or LA47_0 == 81 or LA47_0 == 85 or LA47_0 == 87 or LA47_0 == 91 or LA47_0 == 98 or LA47_0 == 100 or LA47_0 == 102 or LA47_0 == 104 or LA47_0 == 106 or LA47_0 == 110 or LA47_0 == 112 or LA47_0 == 115) :
                    alt47 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 47, 0, self.input)

                    raise nvae


                if alt47 == 1:
                    # osekoil.g:371:7: ID
                    pass 
                    ID87 = self.match(self.input, ID, self.FOLLOW_ID_in_attributeName1504) 
                    if self._state.backtracking == 0:
                        stream_ID.add(ID87)


                    # AST Rewrite
                    # elements: ID
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 371:14: -> ^( ATTRIBUTE_NAME ID )
                        # osekoil.g:371:17: ^( ATTRIBUTE_NAME ID )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(ATTRIBUTE_NAME, "ATTRIBUTE_NAME")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt47 == 2:
                    # osekoil.g:372:7: object
                    pass 
                    self._state.following.append(self.FOLLOW_object_in_attributeName1524)
                    object88 = self.object()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_object.add(object88.tree)


                    # AST Rewrite
                    # elements: object
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 372:14: -> ^( ATTRIBUTE_NAME object )
                        # osekoil.g:372:17: ^( ATTRIBUTE_NAME object )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(ATTRIBUTE_NAME, "ATTRIBUTE_NAME")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_object.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "attributeName"


    class numberRange_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.numberRange_return, self).__init__()

            self.range = None
            self.nfrom = None
            self.nto = None
            self.tree = None





    # $ANTLR start "numberRange"
    # osekoil.g:375:1: numberRange returns [range, nfrom, nto] : ( '[' ( numberList -> ^( NUMBER_RANGE numberList ) | (numberFrom= number range numberTo= number -> ^( NUMBER_RANGE $numberFrom $numberTo) ) ) ']' ) ;
    def numberRange(self, ):
        retval = self.numberRange_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal89 = None
        char_literal92 = None
        numberFrom = None

        numberTo = None

        numberList90 = None

        range91 = None


        char_literal89_tree = None
        char_literal92_tree = None
        stream_RBRACK = RewriteRuleTokenStream(self._adaptor, "token RBRACK")
        stream_LBRACK = RewriteRuleTokenStream(self._adaptor, "token LBRACK")
        stream_range = RewriteRuleSubtreeStream(self._adaptor, "rule range")
        stream_numberList = RewriteRuleSubtreeStream(self._adaptor, "rule numberList")
        stream_number = RewriteRuleSubtreeStream(self._adaptor, "rule number")
        try:
            try:
                # osekoil.g:376:4: ( ( '[' ( numberList -> ^( NUMBER_RANGE numberList ) | (numberFrom= number range numberTo= number -> ^( NUMBER_RANGE $numberFrom $numberTo) ) ) ']' ) )
                # osekoil.g:376:7: ( '[' ( numberList -> ^( NUMBER_RANGE numberList ) | (numberFrom= number range numberTo= number -> ^( NUMBER_RANGE $numberFrom $numberTo) ) ) ']' )
                pass 
                # osekoil.g:376:7: ( '[' ( numberList -> ^( NUMBER_RANGE numberList ) | (numberFrom= number range numberTo= number -> ^( NUMBER_RANGE $numberFrom $numberTo) ) ) ']' )
                # osekoil.g:376:8: '[' ( numberList -> ^( NUMBER_RANGE numberList ) | (numberFrom= number range numberTo= number -> ^( NUMBER_RANGE $numberFrom $numberTo) ) ) ']'
                pass 
                char_literal89 = self.match(self.input, LBRACK, self.FOLLOW_LBRACK_in_numberRange1552) 
                if self._state.backtracking == 0:
                    stream_LBRACK.add(char_literal89)


                # osekoil.g:376:12: ( numberList -> ^( NUMBER_RANGE numberList ) | (numberFrom= number range numberTo= number -> ^( NUMBER_RANGE $numberFrom $numberTo) ) )
                alt48 = 2
                LA48_0 = self.input.LA(1)

                if (LA48_0 == INT) :
                    LA48_1 = self.input.LA(2)

                    if (LA48_1 == RBRACK or LA48_1 == 76) :
                        alt48 = 1
                    elif (LA48_1 == 77) :
                        alt48 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        nvae = NoViableAltException("", 48, 1, self.input)

                        raise nvae


                elif (LA48_0 == HEX) :
                    LA48_2 = self.input.LA(2)

                    if (LA48_2 == RBRACK or LA48_2 == 76) :
                        alt48 = 1
                    elif (LA48_2 == 77) :
                        alt48 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        nvae = NoViableAltException("", 48, 2, self.input)

                        raise nvae


                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 48, 0, self.input)

                    raise nvae


                if alt48 == 1:
                    # osekoil.g:376:13: numberList
                    pass 
                    self._state.following.append(self.FOLLOW_numberList_in_numberRange1555)
                    numberList90 = self.numberList()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_numberList.add(numberList90.tree)


                    if self._state.backtracking == 0:
                        pass
                           
                        retval.nfrom = ((numberList90 is not None) and [numberList90.values] or [None])[0]
                        retval.range = False
                           



                    # AST Rewrite
                    # elements: numberList
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 381:4: -> ^( NUMBER_RANGE numberList )
                        # osekoil.g:381:7: ^( NUMBER_RANGE numberList )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(NUMBER_RANGE, "NUMBER_RANGE")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_numberList.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt48 == 2:
                    # osekoil.g:382:7: (numberFrom= number range numberTo= number -> ^( NUMBER_RANGE $numberFrom $numberTo) )
                    pass 
                    # osekoil.g:382:7: (numberFrom= number range numberTo= number -> ^( NUMBER_RANGE $numberFrom $numberTo) )
                    # osekoil.g:382:8: numberFrom= number range numberTo= number
                    pass 
                    self._state.following.append(self.FOLLOW_number_in_numberRange1584)
                    numberFrom = self.number()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_number.add(numberFrom.tree)


                    if self._state.backtracking == 0:
                        pass
                           
                        retval.range = True
                        retval.nfrom = long(((numberFrom is not None) and [self.input.toString(numberFrom.start,numberFrom.stop)] or [None])[0])
                           



                    self._state.following.append(self.FOLLOW_range_in_numberRange1591)
                    range91 = self.range()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_range.add(range91.tree)


                    self._state.following.append(self.FOLLOW_number_in_numberRange1597)
                    numberTo = self.number()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_number.add(numberTo.tree)


                    if self._state.backtracking == 0:
                        pass
                        retval.nto = long(((numberTo is not None) and [self.input.toString(numberTo.start,numberTo.stop)] or [None])[0])
                           



                    # AST Rewrite
                    # elements: numberTo, numberFrom
                    # token labels: 
                    # rule labels: numberFrom, retval, numberTo
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if numberFrom is not None:
                            stream_numberFrom = RewriteRuleSubtreeStream(self._adaptor, "rule numberFrom", numberFrom.tree)
                        else:
                            stream_numberFrom = RewriteRuleSubtreeStream(self._adaptor, "token numberFrom", None)

                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)

                        if numberTo is not None:
                            stream_numberTo = RewriteRuleSubtreeStream(self._adaptor, "rule numberTo", numberTo.tree)
                        else:
                            stream_numberTo = RewriteRuleSubtreeStream(self._adaptor, "token numberTo", None)


                        root_0 = self._adaptor.nil()
                        # 389:4: -> ^( NUMBER_RANGE $numberFrom $numberTo)
                        # osekoil.g:389:7: ^( NUMBER_RANGE $numberFrom $numberTo)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(NUMBER_RANGE, "NUMBER_RANGE")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_numberFrom.nextTree())

                        self._adaptor.addChild(root_1, stream_numberTo.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0









                char_literal92 = self.match(self.input, RBRACK, self.FOLLOW_RBRACK_in_numberRange1625) 
                if self._state.backtracking == 0:
                    stream_RBRACK.add(char_literal92)







                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "numberRange"


    class numberList_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.numberList_return, self).__init__()

            self.values = None
            self.tree = None





    # $ANTLR start "numberList"
    # osekoil.g:393:1: numberList returns [values] :nums+= number ( ',' nums+= number )* -> ^( NUMBER_LIST ( number )+ ) ;
    def numberList(self, ):
        retval = self.numberList_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal93 = None
        list_nums = None
        nums = None

        nums = None
        char_literal93_tree = None
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_number = RewriteRuleSubtreeStream(self._adaptor, "rule number")
              
        values = []

        try:
            try:
                # osekoil.g:397:4: (nums+= number ( ',' nums+= number )* -> ^( NUMBER_LIST ( number )+ ) )
                # osekoil.g:397:7: nums+= number ( ',' nums+= number )*
                pass 
                self._state.following.append(self.FOLLOW_number_in_numberList1654)
                nums = self.number()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_number.add(nums.tree)

                if list_nums is None:
                    list_nums = []
                list_nums.append(nums.tree)


                # osekoil.g:397:22: ( ',' nums+= number )*
                while True: #loop49
                    alt49 = 2
                    LA49_0 = self.input.LA(1)

                    if (LA49_0 == 76) :
                        alt49 = 1


                    if alt49 == 1:
                        # osekoil.g:397:23: ',' nums+= number
                        pass 
                        char_literal93 = self.match(self.input, 76, self.FOLLOW_76_in_numberList1657) 
                        if self._state.backtracking == 0:
                            stream_76.add(char_literal93)


                        self._state.following.append(self.FOLLOW_number_in_numberList1663)
                        nums = self.number()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_number.add(nums.tree)

                        if list_nums is None:
                            list_nums = []
                        list_nums.append(nums.tree)



                    else:
                        break #loop49


                if self._state.backtracking == 0:
                    pass
                       
                    retval.values = [long(n.text) for n in list_nums]
                        



                # AST Rewrite
                # elements: number
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 401:4: -> ^( NUMBER_LIST ( number )+ )
                    # osekoil.g:401:7: ^( NUMBER_LIST ( number )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(NUMBER_LIST, "NUMBER_LIST")
                    , root_1)

                    # osekoil.g:401:21: ( number )+
                    if not (stream_number.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_number.hasNext():
                        self._adaptor.addChild(root_1, stream_number.nextTree())


                    stream_number.reset()

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "numberList"


    class multipleSpecifier_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.multipleSpecifier_return, self).__init__()

            self.tree = None





    # $ANTLR start "multipleSpecifier"
    # osekoil.g:404:1: multipleSpecifier : ( '[' ']' ) -> ^( MULTIPLE_SPECIFIER ) ;
    def multipleSpecifier(self, ):
        retval = self.multipleSpecifier_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal94 = None
        char_literal95 = None

        char_literal94_tree = None
        char_literal95_tree = None
        stream_RBRACK = RewriteRuleTokenStream(self._adaptor, "token RBRACK")
        stream_LBRACK = RewriteRuleTokenStream(self._adaptor, "token LBRACK")

        try:
            try:
                # osekoil.g:405:4: ( ( '[' ']' ) -> ^( MULTIPLE_SPECIFIER ) )
                # osekoil.g:405:7: ( '[' ']' )
                pass 
                # osekoil.g:405:7: ( '[' ']' )
                # osekoil.g:405:8: '[' ']'
                pass 
                char_literal94 = self.match(self.input, LBRACK, self.FOLLOW_LBRACK_in_multipleSpecifier1699) 
                if self._state.backtracking == 0:
                    stream_LBRACK.add(char_literal94)


                char_literal95 = self.match(self.input, RBRACK, self.FOLLOW_RBRACK_in_multipleSpecifier1701) 
                if self._state.backtracking == 0:
                    stream_RBRACK.add(char_literal95)





                # AST Rewrite
                # elements: 
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 405:17: -> ^( MULTIPLE_SPECIFIER )
                    # osekoil.g:405:20: ^( MULTIPLE_SPECIFIER )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(MULTIPLE_SPECIFIER, "MULTIPLE_SPECIFIER")
                    , root_1)

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "multipleSpecifier"


    class floatRange_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.floatRange_return, self).__init__()

            self.nfrom = None
            self.nto = None
            self.tree = None





    # $ANTLR start "floatRange"
    # osekoil.g:408:1: floatRange returns [nfrom, nto] : ( '[' (numberFrom= FLOAT range numberTo= FLOAT ) ']' ) -> ^( FLOAT_RANGE $numberFrom $numberTo) ;
    def floatRange(self, ):
        retval = self.floatRange_return()
        retval.start = self.input.LT(1)


        root_0 = None

        numberFrom = None
        numberTo = None
        char_literal96 = None
        char_literal98 = None
        range97 = None


        numberFrom_tree = None
        numberTo_tree = None
        char_literal96_tree = None
        char_literal98_tree = None
        stream_RBRACK = RewriteRuleTokenStream(self._adaptor, "token RBRACK")
        stream_LBRACK = RewriteRuleTokenStream(self._adaptor, "token LBRACK")
        stream_FLOAT = RewriteRuleTokenStream(self._adaptor, "token FLOAT")
        stream_range = RewriteRuleSubtreeStream(self._adaptor, "rule range")
        try:
            try:
                # osekoil.g:409:4: ( ( '[' (numberFrom= FLOAT range numberTo= FLOAT ) ']' ) -> ^( FLOAT_RANGE $numberFrom $numberTo) )
                # osekoil.g:409:7: ( '[' (numberFrom= FLOAT range numberTo= FLOAT ) ']' )
                pass 
                # osekoil.g:409:7: ( '[' (numberFrom= FLOAT range numberTo= FLOAT ) ']' )
                # osekoil.g:409:8: '[' (numberFrom= FLOAT range numberTo= FLOAT ) ']'
                pass 
                char_literal96 = self.match(self.input, LBRACK, self.FOLLOW_LBRACK_in_floatRange1728) 
                if self._state.backtracking == 0:
                    stream_LBRACK.add(char_literal96)


                # osekoil.g:409:12: (numberFrom= FLOAT range numberTo= FLOAT )
                # osekoil.g:409:13: numberFrom= FLOAT range numberTo= FLOAT
                pass 
                numberFrom = self.match(self.input, FLOAT, self.FOLLOW_FLOAT_in_floatRange1735) 
                if self._state.backtracking == 0:
                    stream_FLOAT.add(numberFrom)


                if self._state.backtracking == 0:
                    pass
                    retval.nfrom = float(numberFrom.text) 



                self._state.following.append(self.FOLLOW_range_in_floatRange1739)
                range97 = self.range()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_range.add(range97.tree)


                numberTo = self.match(self.input, FLOAT, self.FOLLOW_FLOAT_in_floatRange1745) 
                if self._state.backtracking == 0:
                    stream_FLOAT.add(numberTo)


                if self._state.backtracking == 0:
                    pass
                    retval.nto = float(numberTo.text) 






                char_literal98 = self.match(self.input, RBRACK, self.FOLLOW_RBRACK_in_floatRange1750) 
                if self._state.backtracking == 0:
                    stream_RBRACK.add(char_literal98)





                # AST Rewrite
                # elements: numberTo, numberFrom
                # token labels: numberFrom, numberTo
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    stream_numberFrom = RewriteRuleTokenStream(self._adaptor, "token numberFrom", numberFrom)
                    stream_numberTo = RewriteRuleTokenStream(self._adaptor, "token numberTo", numberTo)
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 410:4: -> ^( FLOAT_RANGE $numberFrom $numberTo)
                    # osekoil.g:410:7: ^( FLOAT_RANGE $numberFrom $numberTo)
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(FLOAT_RANGE, "FLOAT_RANGE")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_numberFrom.nextNode())

                    self._adaptor.addChild(root_1, stream_numberTo.nextNode())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "floatRange"


    class enumeration_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.enumeration_return, self).__init__()

            self.values = None
            self.tree = None





    # $ANTLR start "enumeration"
    # osekoil.g:413:1: enumeration returns [values] : '[' enumeratorList ']' -> ^( ENUMERATION enumeratorList ) ;
    def enumeration(self, ):
        retval = self.enumeration_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal99 = None
        char_literal101 = None
        enumeratorList100 = None


        char_literal99_tree = None
        char_literal101_tree = None
        stream_RBRACK = RewriteRuleTokenStream(self._adaptor, "token RBRACK")
        stream_LBRACK = RewriteRuleTokenStream(self._adaptor, "token LBRACK")
        stream_enumeratorList = RewriteRuleSubtreeStream(self._adaptor, "rule enumeratorList")
        try:
            try:
                # osekoil.g:414:4: ( '[' enumeratorList ']' -> ^( ENUMERATION enumeratorList ) )
                # osekoil.g:414:7: '[' enumeratorList ']'
                pass 
                char_literal99 = self.match(self.input, LBRACK, self.FOLLOW_LBRACK_in_enumeration1785) 
                if self._state.backtracking == 0:
                    stream_LBRACK.add(char_literal99)


                self._state.following.append(self.FOLLOW_enumeratorList_in_enumeration1787)
                enumeratorList100 = self.enumeratorList()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_enumeratorList.add(enumeratorList100.tree)


                char_literal101 = self.match(self.input, RBRACK, self.FOLLOW_RBRACK_in_enumeration1789) 
                if self._state.backtracking == 0:
                    stream_RBRACK.add(char_literal101)


                if self._state.backtracking == 0:
                    pass
                       
                    retval.values = ImplementationDefinition.Enumeration(((enumeratorList100 is not None) and [enumeratorList100.values] or [None])[0])
                       



                # AST Rewrite
                # elements: enumeratorList
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 418:4: -> ^( ENUMERATION enumeratorList )
                    # osekoil.g:418:7: ^( ENUMERATION enumeratorList )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(ENUMERATION, "ENUMERATION")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_enumeratorList.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "enumeration"


    class enumeratorList_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.enumeratorList_return, self).__init__()

            self.values = None
            self.tree = None





    # $ANTLR start "enumeratorList"
    # osekoil.g:421:1: enumeratorList returns [values] :enums+= enumerator ( ',' enums+= enumerator )* -> ^( ENUMERATOR_LIST ( enumerator )+ ) ;
    def enumeratorList(self, ):
        retval = self.enumeratorList_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal102 = None
        list_enums = None
        enums = None

        enums = None
        char_literal102_tree = None
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_enumerator = RewriteRuleSubtreeStream(self._adaptor, "rule enumerator")
              
        myList = []

        try:
            try:
                # osekoil.g:428:4: (enums+= enumerator ( ',' enums+= enumerator )* -> ^( ENUMERATOR_LIST ( enumerator )+ ) )
                # osekoil.g:428:7: enums+= enumerator ( ',' enums+= enumerator )*
                pass 
                self._state.following.append(self.FOLLOW_enumerator_in_enumeratorList1838)
                enums = self.enumerator()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_enumerator.add(enums.tree)

                if list_enums is None:
                    list_enums = []
                list_enums.append(enums.tree)


                if self._state.backtracking == 0:
                    pass
                    myList.append(enums.value)



                # osekoil.g:428:57: ( ',' enums+= enumerator )*
                while True: #loop50
                    alt50 = 2
                    LA50_0 = self.input.LA(1)

                    if (LA50_0 == 76) :
                        alt50 = 1


                    if alt50 == 1:
                        # osekoil.g:428:58: ',' enums+= enumerator
                        pass 
                        char_literal102 = self.match(self.input, 76, self.FOLLOW_76_in_enumeratorList1843) 
                        if self._state.backtracking == 0:
                            stream_76.add(char_literal102)


                        self._state.following.append(self.FOLLOW_enumerator_in_enumeratorList1849)
                        enums = self.enumerator()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_enumerator.add(enums.tree)

                        if list_enums is None:
                            list_enums = []
                        list_enums.append(enums.tree)


                        if self._state.backtracking == 0:
                            pass
                            myList.append(enums.value) 




                    else:
                        break #loop50


                # AST Rewrite
                # elements: enumerator
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 429:4: -> ^( ENUMERATOR_LIST ( enumerator )+ )
                    # osekoil.g:429:7: ^( ENUMERATOR_LIST ( enumerator )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(ENUMERATOR_LIST, "ENUMERATOR_LIST")
                    , root_1)

                    # osekoil.g:429:25: ( enumerator )+
                    if not (stream_enumerator.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_enumerator.hasNext():
                        self._adaptor.addChild(root_1, stream_enumerator.nextTree())


                    stream_enumerator.reset()

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



                if self._state.backtracking == 0:
                    pass
                           
                    retval.values = myList




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "enumeratorList"


    class enumerator_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.enumerator_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "enumerator"
    # osekoil.g:432:1: enumerator returns [value] : ID ( implParameterList )? ( description )? -> ^( ENUMERATOR ID ( implParameterList )? ( description )? ) ;
    def enumerator(self, ):
        retval = self.enumerator_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID103 = None
        implParameterList104 = None

        description105 = None


        ID103_tree = None
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_implParameterList = RewriteRuleSubtreeStream(self._adaptor, "rule implParameterList")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
              


        try:
            try:
                # osekoil.g:439:4: ( ID ( implParameterList )? ( description )? -> ^( ENUMERATOR ID ( implParameterList )? ( description )? ) )
                # osekoil.g:439:7: ID ( implParameterList )? ( description )?
                pass 
                ID103 = self.match(self.input, ID, self.FOLLOW_ID_in_enumerator1894) 
                if self._state.backtracking == 0:
                    stream_ID.add(ID103)


                # osekoil.g:439:10: ( implParameterList )?
                alt51 = 2
                LA51_0 = self.input.LA(1)

                if (LA51_0 == LCURLY) :
                    alt51 = 1
                if alt51 == 1:
                    # osekoil.g:439:10: implParameterList
                    pass 
                    self._state.following.append(self.FOLLOW_implParameterList_in_enumerator1896)
                    implParameterList104 = self.implParameterList()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_implParameterList.add(implParameterList104.tree)





                # osekoil.g:439:29: ( description )?
                alt52 = 2
                LA52_0 = self.input.LA(1)

                if (LA52_0 == 78) :
                    alt52 = 1
                if alt52 == 1:
                    # osekoil.g:439:29: description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_enumerator1899)
                    description105 = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(description105.tree)





                if self._state.backtracking == 0:
                    pass
                       
                    retval.value = ImplementationDefinition.Enumerator(ID103.text, ((implParameterList104 is not None) and [implParameterList104.values] or [None])[0], ((description105 is not None) and [description105.value] or [None])[0])

                       



                # AST Rewrite
                # elements: implParameterList, description, ID
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 444:4: -> ^( ENUMERATOR ID ( implParameterList )? ( description )? )
                    # osekoil.g:444:7: ^( ENUMERATOR ID ( implParameterList )? ( description )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(ENUMERATOR, "ENUMERATOR")
                    , root_1)

                    self._adaptor.addChild(root_1, 
                    stream_ID.nextNode()
                    )

                    # osekoil.g:444:23: ( implParameterList )?
                    if stream_implParameterList.hasNext():
                        self._adaptor.addChild(root_1, stream_implParameterList.nextTree())


                    stream_implParameterList.reset();

                    # osekoil.g:444:42: ( description )?
                    if stream_description.hasNext():
                        self._adaptor.addChild(root_1, stream_description.nextTree())


                    stream_description.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


                if self._state.backtracking == 0:
                    pass
                           





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "enumerator"


    class boolValues_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.boolValues_return, self).__init__()

            self.values = None
            self.tree = None





    # $ANTLR start "boolValues"
    # osekoil.g:447:1: boolValues returns [values] : ( '[' 'TRUE' (tlist= implParameterList )? (tdesc= description )? ',' 'FALSE' (flist= implParameterList )? (fdesc= description )? ']' ) -> ^( BOOL_VALUES ^( 'TRUE' ( $tlist)? ( $tdesc)? ) ^( 'FALSE' ( $flist)? ( $fdesc)? ) ) ;
    def boolValues(self, ):
        retval = self.boolValues_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal106 = None
        string_literal107 = None
        char_literal108 = None
        string_literal109 = None
        char_literal110 = None
        tlist = None

        tdesc = None

        flist = None

        fdesc = None


        char_literal106_tree = None
        string_literal107_tree = None
        char_literal108_tree = None
        string_literal109_tree = None
        char_literal110_tree = None
        stream_RBRACK = RewriteRuleTokenStream(self._adaptor, "token RBRACK")
        stream_117 = RewriteRuleTokenStream(self._adaptor, "token 117")
        stream_LBRACK = RewriteRuleTokenStream(self._adaptor, "token LBRACK")
        stream_93 = RewriteRuleTokenStream(self._adaptor, "token 93")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_implParameterList = RewriteRuleSubtreeStream(self._adaptor, "rule implParameterList")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
              


        try:
            try:
                # osekoil.g:454:4: ( ( '[' 'TRUE' (tlist= implParameterList )? (tdesc= description )? ',' 'FALSE' (flist= implParameterList )? (fdesc= description )? ']' ) -> ^( BOOL_VALUES ^( 'TRUE' ( $tlist)? ( $tdesc)? ) ^( 'FALSE' ( $flist)? ( $fdesc)? ) ) )
                # osekoil.g:454:7: ( '[' 'TRUE' (tlist= implParameterList )? (tdesc= description )? ',' 'FALSE' (flist= implParameterList )? (fdesc= description )? ']' )
                pass 
                # osekoil.g:454:7: ( '[' 'TRUE' (tlist= implParameterList )? (tdesc= description )? ',' 'FALSE' (flist= implParameterList )? (fdesc= description )? ']' )
                # osekoil.g:454:8: '[' 'TRUE' (tlist= implParameterList )? (tdesc= description )? ',' 'FALSE' (flist= implParameterList )? (fdesc= description )? ']'
                pass 
                char_literal106 = self.match(self.input, LBRACK, self.FOLLOW_LBRACK_in_boolValues1952) 
                if self._state.backtracking == 0:
                    stream_LBRACK.add(char_literal106)


                string_literal107 = self.match(self.input, 117, self.FOLLOW_117_in_boolValues1954) 
                if self._state.backtracking == 0:
                    stream_117.add(string_literal107)


                # osekoil.g:454:25: (tlist= implParameterList )?
                alt53 = 2
                LA53_0 = self.input.LA(1)

                if (LA53_0 == LCURLY) :
                    alt53 = 1
                if alt53 == 1:
                    # osekoil.g:454:25: tlist= implParameterList
                    pass 
                    self._state.following.append(self.FOLLOW_implParameterList_in_boolValues1960)
                    tlist = self.implParameterList()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_implParameterList.add(tlist.tree)





                # osekoil.g:454:52: (tdesc= description )?
                alt54 = 2
                LA54_0 = self.input.LA(1)

                if (LA54_0 == 78) :
                    alt54 = 1
                if alt54 == 1:
                    # osekoil.g:454:52: tdesc= description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_boolValues1967)
                    tdesc = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(tdesc.tree)





                char_literal108 = self.match(self.input, 76, self.FOLLOW_76_in_boolValues1970) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal108)


                string_literal109 = self.match(self.input, 93, self.FOLLOW_93_in_boolValues1972) 
                if self._state.backtracking == 0:
                    stream_93.add(string_literal109)


                # osekoil.g:454:85: (flist= implParameterList )?
                alt55 = 2
                LA55_0 = self.input.LA(1)

                if (LA55_0 == LCURLY) :
                    alt55 = 1
                if alt55 == 1:
                    # osekoil.g:454:85: flist= implParameterList
                    pass 
                    self._state.following.append(self.FOLLOW_implParameterList_in_boolValues1978)
                    flist = self.implParameterList()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_implParameterList.add(flist.tree)





                # osekoil.g:454:112: (fdesc= description )?
                alt56 = 2
                LA56_0 = self.input.LA(1)

                if (LA56_0 == 78) :
                    alt56 = 1
                if alt56 == 1:
                    # osekoil.g:454:112: fdesc= description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_boolValues1985)
                    fdesc = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(fdesc.tree)





                char_literal110 = self.match(self.input, RBRACK, self.FOLLOW_RBRACK_in_boolValues1988) 
                if self._state.backtracking == 0:
                    stream_RBRACK.add(char_literal110)





                if self._state.backtracking == 0:
                    pass
                       
                    retval.values = ImplementationDefinition.BoolValues(tlist, tdesc, flist, fdesc)
                       



                # AST Rewrite
                # elements: tlist, tdesc, flist, 117, 93, fdesc
                # token labels: 
                # rule labels: tlist, retval, fdesc, tdesc, flist
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if tlist is not None:
                        stream_tlist = RewriteRuleSubtreeStream(self._adaptor, "rule tlist", tlist.tree)
                    else:
                        stream_tlist = RewriteRuleSubtreeStream(self._adaptor, "token tlist", None)

                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)

                    if fdesc is not None:
                        stream_fdesc = RewriteRuleSubtreeStream(self._adaptor, "rule fdesc", fdesc.tree)
                    else:
                        stream_fdesc = RewriteRuleSubtreeStream(self._adaptor, "token fdesc", None)

                    if tdesc is not None:
                        stream_tdesc = RewriteRuleSubtreeStream(self._adaptor, "rule tdesc", tdesc.tree)
                    else:
                        stream_tdesc = RewriteRuleSubtreeStream(self._adaptor, "token tdesc", None)

                    if flist is not None:
                        stream_flist = RewriteRuleSubtreeStream(self._adaptor, "rule flist", flist.tree)
                    else:
                        stream_flist = RewriteRuleSubtreeStream(self._adaptor, "token flist", None)


                    root_0 = self._adaptor.nil()
                    # 458:4: -> ^( BOOL_VALUES ^( 'TRUE' ( $tlist)? ( $tdesc)? ) ^( 'FALSE' ( $flist)? ( $fdesc)? ) )
                    # osekoil.g:458:7: ^( BOOL_VALUES ^( 'TRUE' ( $tlist)? ( $tdesc)? ) ^( 'FALSE' ( $flist)? ( $fdesc)? ) )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(BOOL_VALUES, "BOOL_VALUES")
                    , root_1)

                    # osekoil.g:458:21: ^( 'TRUE' ( $tlist)? ( $tdesc)? )
                    root_2 = self._adaptor.nil()
                    root_2 = self._adaptor.becomeRoot(
                    stream_117.nextNode()
                    , root_2)

                    # osekoil.g:458:31: ( $tlist)?
                    if stream_tlist.hasNext():
                        self._adaptor.addChild(root_2, stream_tlist.nextTree())


                    stream_tlist.reset();

                    # osekoil.g:458:39: ( $tdesc)?
                    if stream_tdesc.hasNext():
                        self._adaptor.addChild(root_2, stream_tdesc.nextTree())


                    stream_tdesc.reset();

                    self._adaptor.addChild(root_1, root_2)

                    # osekoil.g:458:47: ^( 'FALSE' ( $flist)? ( $fdesc)? )
                    root_2 = self._adaptor.nil()
                    root_2 = self._adaptor.becomeRoot(
                    stream_93.nextNode()
                    , root_2)

                    # osekoil.g:458:58: ( $flist)?
                    if stream_flist.hasNext():
                        self._adaptor.addChild(root_2, stream_flist.nextTree())


                    stream_flist.reset();

                    # osekoil.g:458:66: ( $fdesc)?
                    if stream_fdesc.hasNext():
                        self._adaptor.addChild(root_2, stream_fdesc.nextTree())


                    stream_fdesc.reset();

                    self._adaptor.addChild(root_1, root_2)

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


                if self._state.backtracking == 0:
                    pass
                           





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "boolValues"


    class autoSpecifier_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.autoSpecifier_return, self).__init__()

            self.tree = None





    # $ANTLR start "autoSpecifier"
    # osekoil.g:461:1: autoSpecifier : 'WITH_AUTO' ;
    def autoSpecifier(self, ):
        retval = self.autoSpecifier_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal111 = None

        string_literal111_tree = None

        try:
            try:
                # osekoil.g:462:4: ( 'WITH_AUTO' )
                # osekoil.g:462:7: 'WITH_AUTO'
                pass 
                root_0 = self._adaptor.nil()


                string_literal111 = self.match(self.input, 120, self.FOLLOW_120_in_autoSpecifier2043)
                if self._state.backtracking == 0:
                    string_literal111_tree = self._adaptor.createWithPayload(string_literal111)
                    self._adaptor.addChild(root_0, string_literal111_tree)





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "autoSpecifier"


    class defaultNumber_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.defaultNumber_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "defaultNumber"
    # osekoil.g:465:1: defaultNumber returns [value] : ( ASSIGN (num= number -> ^( DEFAULT_NUMBER $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_NUMBER $id) |id= 'AUTO' -> ^( DEFAULT_NUMBER $id) ) ) ;
    def defaultNumber(self, ):
        retval = self.defaultNumber_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None
        ASSIGN112 = None
        num = None


        id_tree = None
        ASSIGN112_tree = None
        stream_108 = RewriteRuleTokenStream(self._adaptor, "token 108")
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_ASSIGN = RewriteRuleTokenStream(self._adaptor, "token ASSIGN")
        stream_number = RewriteRuleSubtreeStream(self._adaptor, "rule number")
        try:
            try:
                # osekoil.g:466:4: ( ( ASSIGN (num= number -> ^( DEFAULT_NUMBER $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_NUMBER $id) |id= 'AUTO' -> ^( DEFAULT_NUMBER $id) ) ) )
                # osekoil.g:466:7: ( ASSIGN (num= number -> ^( DEFAULT_NUMBER $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_NUMBER $id) |id= 'AUTO' -> ^( DEFAULT_NUMBER $id) ) )
                pass 
                # osekoil.g:466:7: ( ASSIGN (num= number -> ^( DEFAULT_NUMBER $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_NUMBER $id) |id= 'AUTO' -> ^( DEFAULT_NUMBER $id) ) )
                # osekoil.g:466:8: ASSIGN (num= number -> ^( DEFAULT_NUMBER $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_NUMBER $id) |id= 'AUTO' -> ^( DEFAULT_NUMBER $id) )
                pass 
                ASSIGN112 = self.match(self.input, ASSIGN, self.FOLLOW_ASSIGN_in_defaultNumber2064) 
                if self._state.backtracking == 0:
                    stream_ASSIGN.add(ASSIGN112)


                # osekoil.g:466:15: (num= number -> ^( DEFAULT_NUMBER $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_NUMBER $id) |id= 'AUTO' -> ^( DEFAULT_NUMBER $id) )
                alt57 = 3
                LA57 = self.input.LA(1)
                if LA57 == HEX or LA57 == INT:
                    alt57 = 1
                elif LA57 == 108:
                    alt57 = 2
                elif LA57 == 83:
                    alt57 = 3
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 57, 0, self.input)

                    raise nvae


                if alt57 == 1:
                    # osekoil.g:467:4: num= number
                    pass 
                    self._state.following.append(self.FOLLOW_number_in_defaultNumber2075)
                    num = self.number()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_number.add(num.tree)


                    if self._state.backtracking == 0:
                        pass
                        retval.value = long(((num is not None) and [self.input.toString(num.start,num.stop)] or [None])[0]) 



                    # AST Rewrite
                    # elements: num
                    # token labels: 
                    # rule labels: retval, num
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)

                        if num is not None:
                            stream_num = RewriteRuleSubtreeStream(self._adaptor, "rule num", num.tree)
                        else:
                            stream_num = RewriteRuleSubtreeStream(self._adaptor, "token num", None)


                        root_0 = self._adaptor.nil()
                        # 467:47: -> ^( DEFAULT_NUMBER $num)
                        # osekoil.g:467:50: ^( DEFAULT_NUMBER $num)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_NUMBER, "DEFAULT_NUMBER")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_num.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt57 == 2:
                    # osekoil.g:468:6: id= 'NO_DEFAULT'
                    pass 
                    id = self.match(self.input, 108, self.FOLLOW_108_in_defaultNumber2098) 
                    if self._state.backtracking == 0:
                        stream_108.add(id)


                    if self._state.backtracking == 0:
                        pass
                        retval.value = id.text 



                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 468:47: -> ^( DEFAULT_NUMBER $id)
                        # osekoil.g:468:50: ^( DEFAULT_NUMBER $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_NUMBER, "DEFAULT_NUMBER")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt57 == 3:
                    # osekoil.g:469:6: id= 'AUTO'
                    pass 
                    id = self.match(self.input, 83, self.FOLLOW_83_in_defaultNumber2121) 
                    if self._state.backtracking == 0:
                        stream_83.add(id)


                    if self._state.backtracking == 0:
                        pass
                        retval.value = id.text 



                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 469:47: -> ^( DEFAULT_NUMBER $id)
                        # osekoil.g:469:50: ^( DEFAULT_NUMBER $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_NUMBER, "DEFAULT_NUMBER")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0











                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "defaultNumber"


    class defaultFloat_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.defaultFloat_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "defaultFloat"
    # osekoil.g:473:1: defaultFloat returns [value] : ( ASSIGN (num= float -> ^( DEFAULT_FLOAT $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_FLOAT $id) |id= 'AUTO' -> ^( DEFAULT_FLOAT $id) ) ) ;
    def defaultFloat(self, ):
        retval = self.defaultFloat_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None
        ASSIGN113 = None
        num = None


        id_tree = None
        ASSIGN113_tree = None
        stream_108 = RewriteRuleTokenStream(self._adaptor, "token 108")
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_ASSIGN = RewriteRuleTokenStream(self._adaptor, "token ASSIGN")
        stream_float = RewriteRuleSubtreeStream(self._adaptor, "rule float")
        try:
            try:
                # osekoil.g:474:4: ( ( ASSIGN (num= float -> ^( DEFAULT_FLOAT $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_FLOAT $id) |id= 'AUTO' -> ^( DEFAULT_FLOAT $id) ) ) )
                # osekoil.g:474:7: ( ASSIGN (num= float -> ^( DEFAULT_FLOAT $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_FLOAT $id) |id= 'AUTO' -> ^( DEFAULT_FLOAT $id) ) )
                pass 
                # osekoil.g:474:7: ( ASSIGN (num= float -> ^( DEFAULT_FLOAT $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_FLOAT $id) |id= 'AUTO' -> ^( DEFAULT_FLOAT $id) ) )
                # osekoil.g:474:8: ASSIGN (num= float -> ^( DEFAULT_FLOAT $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_FLOAT $id) |id= 'AUTO' -> ^( DEFAULT_FLOAT $id) )
                pass 
                ASSIGN113 = self.match(self.input, ASSIGN, self.FOLLOW_ASSIGN_in_defaultFloat2165) 
                if self._state.backtracking == 0:
                    stream_ASSIGN.add(ASSIGN113)


                # osekoil.g:474:15: (num= float -> ^( DEFAULT_FLOAT $num) |id= 'NO_DEFAULT' -> ^( DEFAULT_FLOAT $id) |id= 'AUTO' -> ^( DEFAULT_FLOAT $id) )
                alt58 = 3
                LA58 = self.input.LA(1)
                if LA58 == FLOAT:
                    alt58 = 1
                elif LA58 == 108:
                    alt58 = 2
                elif LA58 == 83:
                    alt58 = 3
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 58, 0, self.input)

                    raise nvae


                if alt58 == 1:
                    # osekoil.g:475:4: num= float
                    pass 
                    self._state.following.append(self.FOLLOW_float_in_defaultFloat2176)
                    num = self.float()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_float.add(num.tree)


                    if self._state.backtracking == 0:
                        pass
                        retval.value = float(((num is not None) and [self.input.toString(num.start,num.stop)] or [None])[0]) 



                    # AST Rewrite
                    # elements: num
                    # token labels: 
                    # rule labels: retval, num
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)

                        if num is not None:
                            stream_num = RewriteRuleSubtreeStream(self._adaptor, "rule num", num.tree)
                        else:
                            stream_num = RewriteRuleSubtreeStream(self._adaptor, "token num", None)


                        root_0 = self._adaptor.nil()
                        # 475:47: -> ^( DEFAULT_FLOAT $num)
                        # osekoil.g:475:50: ^( DEFAULT_FLOAT $num)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_FLOAT, "DEFAULT_FLOAT")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_num.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt58 == 2:
                    # osekoil.g:476:6: id= 'NO_DEFAULT'
                    pass 
                    id = self.match(self.input, 108, self.FOLLOW_108_in_defaultFloat2199) 
                    if self._state.backtracking == 0:
                        stream_108.add(id)


                    if self._state.backtracking == 0:
                        pass
                        retval.value = id.text 



                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 476:47: -> ^( DEFAULT_FLOAT $id)
                        # osekoil.g:476:50: ^( DEFAULT_FLOAT $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_FLOAT, "DEFAULT_FLOAT")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt58 == 3:
                    # osekoil.g:477:6: id= 'AUTO'
                    pass 
                    id = self.match(self.input, 83, self.FOLLOW_83_in_defaultFloat2222) 
                    if self._state.backtracking == 0:
                        stream_83.add(id)


                    if self._state.backtracking == 0:
                        pass
                        retval.value = id.text 



                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 477:46: -> ^( DEFAULT_FLOAT $id)
                        # osekoil.g:477:49: ^( DEFAULT_FLOAT $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_FLOAT, "DEFAULT_FLOAT")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0











                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "defaultFloat"


    class float_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.float_return, self).__init__()

            self.tree = None





    # $ANTLR start "float"
    # osekoil.g:481:1: float : FLOAT -> ^( FLOAT_DEF FLOAT ) ;
    def float(self, ):
        retval = self.float_return()
        retval.start = self.input.LT(1)


        root_0 = None

        FLOAT114 = None

        FLOAT114_tree = None
        stream_FLOAT = RewriteRuleTokenStream(self._adaptor, "token FLOAT")

        try:
            try:
                # osekoil.g:482:4: ( FLOAT -> ^( FLOAT_DEF FLOAT ) )
                # osekoil.g:482:7: FLOAT
                pass 
                FLOAT114 = self.match(self.input, FLOAT, self.FOLLOW_FLOAT_in_float2261) 
                if self._state.backtracking == 0:
                    stream_FLOAT.add(FLOAT114)


                # AST Rewrite
                # elements: FLOAT
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 483:4: -> ^( FLOAT_DEF FLOAT )
                    # osekoil.g:483:7: ^( FLOAT_DEF FLOAT )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(FLOAT_DEF, "FLOAT_DEF")
                    , root_1)

                    self._adaptor.addChild(root_1, 
                    stream_FLOAT.nextNode()
                    )

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "float"


    class defaultName_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.defaultName_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "defaultName"
    # osekoil.g:486:1: defaultName returns [value] : ( ASSIGN (id= ID |id= 'NO_DEFAULT' |id= 'AUTO' ) -> ^( DEFAULT_NAME $id) ) ;
    def defaultName(self, ):
        retval = self.defaultName_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None
        ASSIGN115 = None

        id_tree = None
        ASSIGN115_tree = None
        stream_108 = RewriteRuleTokenStream(self._adaptor, "token 108")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_ASSIGN = RewriteRuleTokenStream(self._adaptor, "token ASSIGN")

        try:
            try:
                # osekoil.g:487:4: ( ( ASSIGN (id= ID |id= 'NO_DEFAULT' |id= 'AUTO' ) -> ^( DEFAULT_NAME $id) ) )
                # osekoil.g:487:7: ( ASSIGN (id= ID |id= 'NO_DEFAULT' |id= 'AUTO' ) -> ^( DEFAULT_NAME $id) )
                pass 
                # osekoil.g:487:7: ( ASSIGN (id= ID |id= 'NO_DEFAULT' |id= 'AUTO' ) -> ^( DEFAULT_NAME $id) )
                # osekoil.g:487:8: ASSIGN (id= ID |id= 'NO_DEFAULT' |id= 'AUTO' )
                pass 
                ASSIGN115 = self.match(self.input, ASSIGN, self.FOLLOW_ASSIGN_in_defaultName2292) 
                if self._state.backtracking == 0:
                    stream_ASSIGN.add(ASSIGN115)


                # osekoil.g:487:15: (id= ID |id= 'NO_DEFAULT' |id= 'AUTO' )
                alt59 = 3
                LA59 = self.input.LA(1)
                if LA59 == ID:
                    alt59 = 1
                elif LA59 == 108:
                    alt59 = 2
                elif LA59 == 83:
                    alt59 = 3
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 59, 0, self.input)

                    raise nvae


                if alt59 == 1:
                    # osekoil.g:488:4: id= ID
                    pass 
                    id = self.match(self.input, ID, self.FOLLOW_ID_in_defaultName2303) 
                    if self._state.backtracking == 0:
                        stream_ID.add(id)



                elif alt59 == 2:
                    # osekoil.g:489:6: id= 'NO_DEFAULT'
                    pass 
                    id = self.match(self.input, 108, self.FOLLOW_108_in_defaultName2314) 
                    if self._state.backtracking == 0:
                        stream_108.add(id)



                elif alt59 == 3:
                    # osekoil.g:490:6: id= 'AUTO'
                    pass 
                    id = self.match(self.input, 83, self.FOLLOW_83_in_defaultName2325) 
                    if self._state.backtracking == 0:
                        stream_83.add(id)





                # AST Rewrite
                # elements: id
                # token labels: id
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 492:4: -> ^( DEFAULT_NAME $id)
                    # osekoil.g:492:7: ^( DEFAULT_NAME $id)
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(DEFAULT_NAME, "DEFAULT_NAME")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_id.nextNode())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0






                if self._state.backtracking == 0:
                    pass
                    retval.value = id.text 





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "defaultName"


    class defaultString_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.defaultString_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "defaultString"
    # osekoil.g:497:1: defaultString returns [value] : ( ASSIGN (id= STRING |id= 'NO_DEFAULT' |id= 'AUTO' ) -> ^( DEFAULT_STRING $id) ) ;
    def defaultString(self, ):
        retval = self.defaultString_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None
        ASSIGN116 = None

        id_tree = None
        ASSIGN116_tree = None
        stream_108 = RewriteRuleTokenStream(self._adaptor, "token 108")
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_STRING = RewriteRuleTokenStream(self._adaptor, "token STRING")
        stream_ASSIGN = RewriteRuleTokenStream(self._adaptor, "token ASSIGN")

        try:
            try:
                # osekoil.g:498:4: ( ( ASSIGN (id= STRING |id= 'NO_DEFAULT' |id= 'AUTO' ) -> ^( DEFAULT_STRING $id) ) )
                # osekoil.g:498:7: ( ASSIGN (id= STRING |id= 'NO_DEFAULT' |id= 'AUTO' ) -> ^( DEFAULT_STRING $id) )
                pass 
                # osekoil.g:498:7: ( ASSIGN (id= STRING |id= 'NO_DEFAULT' |id= 'AUTO' ) -> ^( DEFAULT_STRING $id) )
                # osekoil.g:498:8: ASSIGN (id= STRING |id= 'NO_DEFAULT' |id= 'AUTO' )
                pass 
                ASSIGN116 = self.match(self.input, ASSIGN, self.FOLLOW_ASSIGN_in_defaultString2372) 
                if self._state.backtracking == 0:
                    stream_ASSIGN.add(ASSIGN116)


                # osekoil.g:498:15: (id= STRING |id= 'NO_DEFAULT' |id= 'AUTO' )
                alt60 = 3
                LA60 = self.input.LA(1)
                if LA60 == STRING:
                    alt60 = 1
                elif LA60 == 108:
                    alt60 = 2
                elif LA60 == 83:
                    alt60 = 3
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 60, 0, self.input)

                    raise nvae


                if alt60 == 1:
                    # osekoil.g:499:4: id= STRING
                    pass 
                    id = self.match(self.input, STRING, self.FOLLOW_STRING_in_defaultString2383) 
                    if self._state.backtracking == 0:
                        stream_STRING.add(id)



                elif alt60 == 2:
                    # osekoil.g:500:6: id= 'NO_DEFAULT'
                    pass 
                    id = self.match(self.input, 108, self.FOLLOW_108_in_defaultString2394) 
                    if self._state.backtracking == 0:
                        stream_108.add(id)



                elif alt60 == 3:
                    # osekoil.g:501:6: id= 'AUTO'
                    pass 
                    id = self.match(self.input, 83, self.FOLLOW_83_in_defaultString2405) 
                    if self._state.backtracking == 0:
                        stream_83.add(id)





                # AST Rewrite
                # elements: id
                # token labels: id
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 503:4: -> ^( DEFAULT_STRING $id)
                    # osekoil.g:503:7: ^( DEFAULT_STRING $id)
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(DEFAULT_STRING, "DEFAULT_STRING")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_id.nextNode())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0






                if self._state.backtracking == 0:
                    pass
                    retval.value = id.text 





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "defaultString"


    class defaultBool_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.defaultBool_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "defaultBool"
    # osekoil.g:508:1: defaultBool returns [value] : ( ASSIGN ( boolean -> ^( DEFAULT_BOOL boolean ) |id= 'NO_DEFAULT' -> ^( DEFAULT_BOOL $id) |id= 'AUTO' -> ^( DEFAULT_BOOL $id) ) ) ;
    def defaultBool(self, ):
        retval = self.defaultBool_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None
        ASSIGN117 = None
        boolean118 = None


        id_tree = None
        ASSIGN117_tree = None
        stream_108 = RewriteRuleTokenStream(self._adaptor, "token 108")
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_ASSIGN = RewriteRuleTokenStream(self._adaptor, "token ASSIGN")
        stream_boolean = RewriteRuleSubtreeStream(self._adaptor, "rule boolean")
        try:
            try:
                # osekoil.g:509:4: ( ( ASSIGN ( boolean -> ^( DEFAULT_BOOL boolean ) |id= 'NO_DEFAULT' -> ^( DEFAULT_BOOL $id) |id= 'AUTO' -> ^( DEFAULT_BOOL $id) ) ) )
                # osekoil.g:509:7: ( ASSIGN ( boolean -> ^( DEFAULT_BOOL boolean ) |id= 'NO_DEFAULT' -> ^( DEFAULT_BOOL $id) |id= 'AUTO' -> ^( DEFAULT_BOOL $id) ) )
                pass 
                # osekoil.g:509:7: ( ASSIGN ( boolean -> ^( DEFAULT_BOOL boolean ) |id= 'NO_DEFAULT' -> ^( DEFAULT_BOOL $id) |id= 'AUTO' -> ^( DEFAULT_BOOL $id) ) )
                # osekoil.g:509:8: ASSIGN ( boolean -> ^( DEFAULT_BOOL boolean ) |id= 'NO_DEFAULT' -> ^( DEFAULT_BOOL $id) |id= 'AUTO' -> ^( DEFAULT_BOOL $id) )
                pass 
                ASSIGN117 = self.match(self.input, ASSIGN, self.FOLLOW_ASSIGN_in_defaultBool2452) 
                if self._state.backtracking == 0:
                    stream_ASSIGN.add(ASSIGN117)


                # osekoil.g:509:15: ( boolean -> ^( DEFAULT_BOOL boolean ) |id= 'NO_DEFAULT' -> ^( DEFAULT_BOOL $id) |id= 'AUTO' -> ^( DEFAULT_BOOL $id) )
                alt61 = 3
                LA61 = self.input.LA(1)
                if LA61 == 93 or LA61 == 117:
                    alt61 = 1
                elif LA61 == 108:
                    alt61 = 2
                elif LA61 == 83:
                    alt61 = 3
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 61, 0, self.input)

                    raise nvae


                if alt61 == 1:
                    # osekoil.g:510:4: boolean
                    pass 
                    self._state.following.append(self.FOLLOW_boolean_in_defaultBool2459)
                    boolean118 = self.boolean()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_boolean.add(boolean118.tree)


                    # AST Rewrite
                    # elements: boolean
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 510:20: -> ^( DEFAULT_BOOL boolean )
                        # osekoil.g:510:23: ^( DEFAULT_BOOL boolean )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_BOOL, "DEFAULT_BOOL")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_boolean.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt61 == 2:
                    # osekoil.g:511:6: id= 'NO_DEFAULT'
                    pass 
                    id = self.match(self.input, 108, self.FOLLOW_108_in_defaultBool2486) 
                    if self._state.backtracking == 0:
                        stream_108.add(id)


                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 511:24: -> ^( DEFAULT_BOOL $id)
                        # osekoil.g:511:27: ^( DEFAULT_BOOL $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_BOOL, "DEFAULT_BOOL")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt61 == 3:
                    # osekoil.g:512:6: id= 'AUTO'
                    pass 
                    id = self.match(self.input, 83, self.FOLLOW_83_in_defaultBool2506) 
                    if self._state.backtracking == 0:
                        stream_83.add(id)


                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 512:24: -> ^( DEFAULT_BOOL $id)
                        # osekoil.g:512:27: ^( DEFAULT_BOOL $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(DEFAULT_BOOL, "DEFAULT_BOOL")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0











                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "defaultBool"


    class implParameterList_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implParameterList_return, self).__init__()

            self.values = None
            self.tree = None





    # $ANTLR start "implParameterList"
    # osekoil.g:516:1: implParameterList returns [values] : ( '{' implDefList '}' ) -> ^( IMPL_PARAMETER_LIST implDefList ) ;
    def implParameterList(self, ):
        retval = self.implParameterList_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal119 = None
        char_literal121 = None
        implDefList120 = None


        char_literal119_tree = None
        char_literal121_tree = None
        stream_LCURLY = RewriteRuleTokenStream(self._adaptor, "token LCURLY")
        stream_RCURLY = RewriteRuleTokenStream(self._adaptor, "token RCURLY")
        stream_implDefList = RewriteRuleSubtreeStream(self._adaptor, "rule implDefList")
        try:
            try:
                # osekoil.g:517:4: ( ( '{' implDefList '}' ) -> ^( IMPL_PARAMETER_LIST implDefList ) )
                # osekoil.g:517:7: ( '{' implDefList '}' )
                pass 
                # osekoil.g:517:7: ( '{' implDefList '}' )
                # osekoil.g:517:8: '{' implDefList '}'
                pass 
                char_literal119 = self.match(self.input, LCURLY, self.FOLLOW_LCURLY_in_implParameterList2547) 
                if self._state.backtracking == 0:
                    stream_LCURLY.add(char_literal119)


                self._state.following.append(self.FOLLOW_implDefList_in_implParameterList2549)
                implDefList120 = self.implDefList()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_implDefList.add(implDefList120.tree)


                if self._state.backtracking == 0:
                    pass
                       
                    retval.values = ImplementationDefinition.ImplParameterList(((implDefList120 is not None) and [implDefList120.values] or [None])[0])
                       



                char_literal121 = self.match(self.input, RCURLY, self.FOLLOW_RCURLY_in_implParameterList2556) 
                if self._state.backtracking == 0:
                    stream_RCURLY.add(char_literal121)





                # AST Rewrite
                # elements: implDefList
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 521:4: -> ^( IMPL_PARAMETER_LIST implDefList )
                    # osekoil.g:521:7: ^( IMPL_PARAMETER_LIST implDefList )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IMPL_PARAMETER_LIST, "IMPL_PARAMETER_LIST")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_implDefList.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "implParameterList"


    class implDefList_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implDefList_return, self).__init__()

            self.values = None
            self.tree = None





    # $ANTLR start "implDefList"
    # osekoil.g:524:1: implDefList returns [values] : (defs+= ( implementationDef ) )* -> ^( IMPL_DEF_LIST ( implementationDef )* ) ;
    def implDefList(self, ):
        retval = self.implDefList_return()
        retval.start = self.input.LT(1)


        root_0 = None

        defs = None
        list_defs = None
        implementationDef122 = None


        defs_tree = None
        stream_implementationDef = RewriteRuleSubtreeStream(self._adaptor, "rule implementationDef")
              
        myList = []

        try:
            try:
                # osekoil.g:531:4: ( (defs+= ( implementationDef ) )* -> ^( IMPL_DEF_LIST ( implementationDef )* ) )
                # osekoil.g:531:7: (defs+= ( implementationDef ) )*
                pass 
                # osekoil.g:531:12: (defs+= ( implementationDef ) )*
                while True: #loop62
                    alt62 = 2
                    LA62_0 = self.input.LA(1)

                    if (LA62_0 == 80 or LA62_0 == 82 or LA62_0 == 84 or LA62_0 == 86 or LA62_0 == 88 or LA62_0 == 90 or LA62_0 == 92 or LA62_0 == 94 or (96 <= LA62_0 <= 97) or LA62_0 == 99 or LA62_0 == 101 or LA62_0 == 103 or LA62_0 == 105 or LA62_0 == 107 or LA62_0 == 111 or (113 <= LA62_0 <= 114) or LA62_0 == 116 or (118 <= LA62_0 <= 119)) :
                        alt62 = 1


                    if alt62 == 1:
                        # osekoil.g:531:12: defs+= ( implementationDef )
                        pass 
                        # osekoil.g:531:15: ( implementationDef )
                        # osekoil.g:531:16: implementationDef
                        pass 
                        self._state.following.append(self.FOLLOW_implementationDef_in_implDefList2602)
                        implementationDef122 = self.implementationDef()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_implementationDef.add(implementationDef122.tree)


                        if self._state.backtracking == 0:
                            pass
                            myList.append(((implementationDef122 is not None) and [implementationDef122.values] or [None])[0]), 







                    else:
                        break #loop62


                if self._state.backtracking == 0:
                    pass
                       

                       



                # AST Rewrite
                # elements: implementationDef
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 535:4: -> ^( IMPL_DEF_LIST ( implementationDef )* )
                    # osekoil.g:535:7: ^( IMPL_DEF_LIST ( implementationDef )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IMPL_DEF_LIST, "IMPL_DEF_LIST")
                    , root_1)

                    # osekoil.g:535:23: ( implementationDef )*
                    while stream_implementationDef.hasNext():
                        self._adaptor.addChild(root_1, stream_implementationDef.nextTree())


                    stream_implementationDef.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



                if self._state.backtracking == 0:
                    pass
                           
                    retval.values = myList




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "implDefList"


    class implRefDef_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.implRefDef_return, self).__init__()

            self.objectType = None
            self.name = None
            self.mult = None
            self.desc = None
            self.tree = None





    # $ANTLR start "implRefDef"
    # osekoil.g:539:1: implRefDef returns [objectType, name, mult, desc] : objectRefType referenceName ( multipleSpecifier )? ( description )? SEMI -> ^( IMPL_REF_DEF objectRefType referenceName ( multipleSpecifier )? ( description )? ) ;
    def implRefDef(self, ):
        retval = self.implRefDef_return()
        retval.start = self.input.LT(1)


        root_0 = None

        SEMI127 = None
        objectRefType123 = None

        referenceName124 = None

        multipleSpecifier125 = None

        description126 = None


        SEMI127_tree = None
        stream_SEMI = RewriteRuleTokenStream(self._adaptor, "token SEMI")
        stream_objectRefType = RewriteRuleSubtreeStream(self._adaptor, "rule objectRefType")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
        stream_multipleSpecifier = RewriteRuleSubtreeStream(self._adaptor, "rule multipleSpecifier")
        stream_referenceName = RewriteRuleSubtreeStream(self._adaptor, "rule referenceName")
        try:
            try:
                # osekoil.g:540:4: ( objectRefType referenceName ( multipleSpecifier )? ( description )? SEMI -> ^( IMPL_REF_DEF objectRefType referenceName ( multipleSpecifier )? ( description )? ) )
                # osekoil.g:540:7: objectRefType referenceName ( multipleSpecifier )? ( description )? SEMI
                pass 
                self._state.following.append(self.FOLLOW_objectRefType_in_implRefDef2644)
                objectRefType123 = self.objectRefType()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_objectRefType.add(objectRefType123.tree)


                self._state.following.append(self.FOLLOW_referenceName_in_implRefDef2646)
                referenceName124 = self.referenceName()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_referenceName.add(referenceName124.tree)


                # osekoil.g:540:35: ( multipleSpecifier )?
                alt63 = 2
                LA63_0 = self.input.LA(1)

                if (LA63_0 == LBRACK) :
                    alt63 = 1
                if alt63 == 1:
                    # osekoil.g:540:35: multipleSpecifier
                    pass 
                    self._state.following.append(self.FOLLOW_multipleSpecifier_in_implRefDef2648)
                    multipleSpecifier125 = self.multipleSpecifier()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_multipleSpecifier.add(multipleSpecifier125.tree)





                # osekoil.g:540:54: ( description )?
                alt64 = 2
                LA64_0 = self.input.LA(1)

                if (LA64_0 == 78) :
                    alt64 = 1
                if alt64 == 1:
                    # osekoil.g:540:54: description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_implRefDef2651)
                    description126 = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(description126.tree)





                SEMI127 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_implRefDef2654) 
                if self._state.backtracking == 0:
                    stream_SEMI.add(SEMI127)


                if self._state.backtracking == 0:
                    pass
                       
                    retval.objectType = ((objectRefType123 is not None) and [self.input.toString(objectRefType123.start,objectRefType123.stop)] or [None])[0]
                    retval.name = ((referenceName124 is not None) and [self.input.toString(referenceName124.start,referenceName124.stop)] or [None])[0]
                    if ((multipleSpecifier125 is not None) and [self.input.toString(multipleSpecifier125.start,multipleSpecifier125.stop)] or [None])[0]: retval.mult = True
                    else: retval.mult = False
                    if ((description126 is not None) and [description126.value] or [None])[0]: retval.desc = ((description126 is not None) and [description126.value] or [None])[0]
                    else: retval.desc = ""

                       



                # AST Rewrite
                # elements: description, objectRefType, referenceName, multipleSpecifier
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 550:7: -> ^( IMPL_REF_DEF objectRefType referenceName ( multipleSpecifier )? ( description )? )
                    # osekoil.g:550:10: ^( IMPL_REF_DEF objectRefType referenceName ( multipleSpecifier )? ( description )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IMPL_REF_DEF, "IMPL_REF_DEF")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_objectRefType.nextTree())

                    self._adaptor.addChild(root_1, stream_referenceName.nextTree())

                    # osekoil.g:550:53: ( multipleSpecifier )?
                    if stream_multipleSpecifier.hasNext():
                        self._adaptor.addChild(root_1, stream_multipleSpecifier.nextTree())


                    stream_multipleSpecifier.reset();

                    # osekoil.g:550:72: ( description )?
                    if stream_description.hasNext():
                        self._adaptor.addChild(root_1, stream_description.nextTree())


                    stream_description.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "implRefDef"


    class objectRefType_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.objectRefType_return, self).__init__()

            self.tree = None





    # $ANTLR start "objectRefType"
    # osekoil.g:553:1: objectRefType : (id= 'OS_TYPE' |id= 'TASK_TYPE' |id= 'COUNTER_TYPE' |id= 'ALARM_TYPE' |id= 'RESOURCE_TYPE' |id= 'EVENT_TYPE' |id= 'ISR_TYPE' |id= 'MESSAGE_TYPE' |id= 'COM_TYPE' |id= 'NM_TYPE' |id= 'APPMODE_TYPE' |id= 'IPDU_TYPE' |id= 'NETWORKMESSAGE_TYPE' -> ^( OBJECT_REF_TYPE $id) );
    def objectRefType(self, ):
        retval = self.objectRefType_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None

        id_tree = None
        stream_105 = RewriteRuleTokenStream(self._adaptor, "token 105")

        try:
            try:
                # osekoil.g:554:4: (id= 'OS_TYPE' |id= 'TASK_TYPE' |id= 'COUNTER_TYPE' |id= 'ALARM_TYPE' |id= 'RESOURCE_TYPE' |id= 'EVENT_TYPE' |id= 'ISR_TYPE' |id= 'MESSAGE_TYPE' |id= 'COM_TYPE' |id= 'NM_TYPE' |id= 'APPMODE_TYPE' |id= 'IPDU_TYPE' |id= 'NETWORKMESSAGE_TYPE' -> ^( OBJECT_REF_TYPE $id) )
                alt65 = 13
                LA65 = self.input.LA(1)
                if LA65 == 111:
                    alt65 = 1
                elif LA65 == 116:
                    alt65 = 2
                elif LA65 == 88:
                    alt65 = 3
                elif LA65 == 80:
                    alt65 = 4
                elif LA65 == 113:
                    alt65 = 5
                elif LA65 == 92:
                    alt65 = 6
                elif LA65 == 101:
                    alt65 = 7
                elif LA65 == 103:
                    alt65 = 8
                elif LA65 == 86:
                    alt65 = 9
                elif LA65 == 107:
                    alt65 = 10
                elif LA65 == 82:
                    alt65 = 11
                elif LA65 == 99:
                    alt65 = 12
                elif LA65 == 105:
                    alt65 = 13
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 65, 0, self.input)

                    raise nvae


                if alt65 == 1:
                    # osekoil.g:554:7: id= 'OS_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 111, self.FOLLOW_111_in_objectRefType2701)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 2:
                    # osekoil.g:555:7: id= 'TASK_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 116, self.FOLLOW_116_in_objectRefType2713)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 3:
                    # osekoil.g:556:7: id= 'COUNTER_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 88, self.FOLLOW_88_in_objectRefType2725)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 4:
                    # osekoil.g:557:7: id= 'ALARM_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 80, self.FOLLOW_80_in_objectRefType2737)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 5:
                    # osekoil.g:558:7: id= 'RESOURCE_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 113, self.FOLLOW_113_in_objectRefType2749)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 6:
                    # osekoil.g:559:7: id= 'EVENT_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 92, self.FOLLOW_92_in_objectRefType2761)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 7:
                    # osekoil.g:560:7: id= 'ISR_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 101, self.FOLLOW_101_in_objectRefType2773)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 8:
                    # osekoil.g:561:7: id= 'MESSAGE_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 103, self.FOLLOW_103_in_objectRefType2785)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 9:
                    # osekoil.g:562:7: id= 'COM_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 86, self.FOLLOW_86_in_objectRefType2797)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 10:
                    # osekoil.g:563:7: id= 'NM_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 107, self.FOLLOW_107_in_objectRefType2809)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 11:
                    # osekoil.g:564:7: id= 'APPMODE_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 82, self.FOLLOW_82_in_objectRefType2821)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 12:
                    # osekoil.g:565:7: id= 'IPDU_TYPE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 99, self.FOLLOW_99_in_objectRefType2833)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)




                elif alt65 == 13:
                    # osekoil.g:566:7: id= 'NETWORKMESSAGE_TYPE'
                    pass 
                    id = self.match(self.input, 105, self.FOLLOW_105_in_objectRefType2845) 
                    if self._state.backtracking == 0:
                        stream_105.add(id)


                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 567:4: -> ^( OBJECT_REF_TYPE $id)
                        # osekoil.g:567:7: ^( OBJECT_REF_TYPE $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(OBJECT_REF_TYPE, "OBJECT_REF_TYPE")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "objectRefType"


    class referenceName_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.referenceName_return, self).__init__()

            self.tree = None





    # $ANTLR start "referenceName"
    # osekoil.g:570:1: referenceName : ( ID -> ^( REFERENCE_NAME ID ) | object -> ^( REFERENCE_NAME object ) );
    def referenceName(self, ):
        retval = self.referenceName_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID128 = None
        object129 = None


        ID128_tree = None
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_object = RewriteRuleSubtreeStream(self._adaptor, "rule object")
        try:
            try:
                # osekoil.g:571:4: ( ID -> ^( REFERENCE_NAME ID ) | object -> ^( REFERENCE_NAME object ) )
                alt66 = 2
                LA66_0 = self.input.LA(1)

                if (LA66_0 == ID) :
                    alt66 = 1
                elif (LA66_0 == 79 or LA66_0 == 81 or LA66_0 == 85 or LA66_0 == 87 or LA66_0 == 91 or LA66_0 == 98 or LA66_0 == 100 or LA66_0 == 102 or LA66_0 == 104 or LA66_0 == 106 or LA66_0 == 110 or LA66_0 == 112 or LA66_0 == 115) :
                    alt66 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 66, 0, self.input)

                    raise nvae


                if alt66 == 1:
                    # osekoil.g:571:7: ID
                    pass 
                    ID128 = self.match(self.input, ID, self.FOLLOW_ID_in_referenceName2873) 
                    if self._state.backtracking == 0:
                        stream_ID.add(ID128)


                    # AST Rewrite
                    # elements: ID
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 571:10: -> ^( REFERENCE_NAME ID )
                        # osekoil.g:571:13: ^( REFERENCE_NAME ID )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(REFERENCE_NAME, "REFERENCE_NAME")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt66 == 2:
                    # osekoil.g:572:7: object
                    pass 
                    self._state.following.append(self.FOLLOW_object_in_referenceName2889)
                    object129 = self.object()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_object.add(object129.tree)


                    # AST Rewrite
                    # elements: object
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 572:16: -> ^( REFERENCE_NAME object )
                        # osekoil.g:572:19: ^( REFERENCE_NAME object )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(REFERENCE_NAME, "REFERENCE_NAME")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_object.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "referenceName"


    class applicationDefinition_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.applicationDefinition_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "applicationDefinition"
    # osekoil.g:580:1: applicationDefinition returns [value] : 'CPU' ID '{' objectDefinitionList '}' ( description )? SEMI -> ^( APPLICATION_DEFINITION objectDefinitionList ( description )? ) ;
    def applicationDefinition(self, ):
        retval = self.applicationDefinition_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal130 = None
        ID131 = None
        char_literal132 = None
        char_literal134 = None
        SEMI136 = None
        objectDefinitionList133 = None

        description135 = None


        string_literal130_tree = None
        ID131_tree = None
        char_literal132_tree = None
        char_literal134_tree = None
        SEMI136_tree = None
        stream_LCURLY = RewriteRuleTokenStream(self._adaptor, "token LCURLY")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_SEMI = RewriteRuleTokenStream(self._adaptor, "token SEMI")
        stream_RCURLY = RewriteRuleTokenStream(self._adaptor, "token RCURLY")
        stream_89 = RewriteRuleTokenStream(self._adaptor, "token 89")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
        stream_objectDefinitionList = RewriteRuleSubtreeStream(self._adaptor, "rule objectDefinitionList")
        try:
            try:
                # osekoil.g:581:4: ( 'CPU' ID '{' objectDefinitionList '}' ( description )? SEMI -> ^( APPLICATION_DEFINITION objectDefinitionList ( description )? ) )
                # osekoil.g:581:7: 'CPU' ID '{' objectDefinitionList '}' ( description )? SEMI
                pass 
                string_literal130 = self.match(self.input, 89, self.FOLLOW_89_in_applicationDefinition2923) 
                if self._state.backtracking == 0:
                    stream_89.add(string_literal130)


                ID131 = self.match(self.input, ID, self.FOLLOW_ID_in_applicationDefinition2925) 
                if self._state.backtracking == 0:
                    stream_ID.add(ID131)


                char_literal132 = self.match(self.input, LCURLY, self.FOLLOW_LCURLY_in_applicationDefinition2927) 
                if self._state.backtracking == 0:
                    stream_LCURLY.add(char_literal132)


                self._state.following.append(self.FOLLOW_objectDefinitionList_in_applicationDefinition2929)
                objectDefinitionList133 = self.objectDefinitionList()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_objectDefinitionList.add(objectDefinitionList133.tree)


                char_literal134 = self.match(self.input, RCURLY, self.FOLLOW_RCURLY_in_applicationDefinition2931) 
                if self._state.backtracking == 0:
                    stream_RCURLY.add(char_literal134)


                # osekoil.g:581:45: ( description )?
                alt67 = 2
                LA67_0 = self.input.LA(1)

                if (LA67_0 == 78) :
                    alt67 = 1
                if alt67 == 1:
                    # osekoil.g:581:45: description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_applicationDefinition2933)
                    description135 = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(description135.tree)





                SEMI136 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_applicationDefinition2936) 
                if self._state.backtracking == 0:
                    stream_SEMI.add(SEMI136)


                if self._state.backtracking == 0:
                    pass
                       
                    retval.value = ApplicationDefinition.ApplicationDefinitionBuilder(self, ((objectDefinitionList133 is not None) and [objectDefinitionList133.value] or [None])[0], ((description135 is not None) and [description135.value] or [None])[0])
                       



                # AST Rewrite
                # elements: description, objectDefinitionList
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 585:4: -> ^( APPLICATION_DEFINITION objectDefinitionList ( description )? )
                    # osekoil.g:585:8: ^( APPLICATION_DEFINITION objectDefinitionList ( description )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(APPLICATION_DEFINITION, "APPLICATION_DEFINITION")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_objectDefinitionList.nextTree())

                    # osekoil.g:585:54: ( description )?
                    if stream_description.hasNext():
                        self._adaptor.addChild(root_1, stream_description.nextTree())


                    stream_description.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "applicationDefinition"


    class objectDefinitionList_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.objectDefinitionList_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "objectDefinitionList"
    # osekoil.g:589:1: objectDefinitionList returns [value] : (defs+= ( objectDefinition ) )* -> ^( OBJECT_DEFINITION_LIST ( objectDefinition )* ) ;
    def objectDefinitionList(self, ):
        retval = self.objectDefinitionList_return()
        retval.start = self.input.LT(1)


        root_0 = None

        defs = None
        list_defs = None
        objectDefinition137 = None


        defs_tree = None
        stream_objectDefinition = RewriteRuleSubtreeStream(self._adaptor, "rule objectDefinition")
              
        myList = []

        try:
            try:
                # osekoil.g:597:4: ( (defs+= ( objectDefinition ) )* -> ^( OBJECT_DEFINITION_LIST ( objectDefinition )* ) )
                # osekoil.g:597:7: (defs+= ( objectDefinition ) )*
                pass 
                # osekoil.g:597:12: (defs+= ( objectDefinition ) )*
                while True: #loop68
                    alt68 = 2
                    LA68_0 = self.input.LA(1)

                    if (LA68_0 == 79 or LA68_0 == 81 or LA68_0 == 85 or LA68_0 == 87 or LA68_0 == 91 or LA68_0 == 98 or LA68_0 == 100 or LA68_0 == 102 or LA68_0 == 104 or LA68_0 == 106 or LA68_0 == 110 or LA68_0 == 112 or LA68_0 == 115) :
                        alt68 = 1


                    if alt68 == 1:
                        # osekoil.g:597:12: defs+= ( objectDefinition )
                        pass 
                        # osekoil.g:597:15: ( objectDefinition )
                        # osekoil.g:597:16: objectDefinition
                        pass 
                        self._state.following.append(self.FOLLOW_objectDefinition_in_objectDefinitionList2991)
                        objectDefinition137 = self.objectDefinition()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_objectDefinition.add(objectDefinition137.tree)


                        if self._state.backtracking == 0:
                            pass
                            myList.append(((objectDefinition137 is not None) and [objectDefinition137.value] or [None])[0]) 







                    else:
                        break #loop68


                # AST Rewrite
                # elements: objectDefinition
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 598:4: -> ^( OBJECT_DEFINITION_LIST ( objectDefinition )* )
                    # osekoil.g:598:7: ^( OBJECT_DEFINITION_LIST ( objectDefinition )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(OBJECT_DEFINITION_LIST, "OBJECT_DEFINITION_LIST")
                    , root_1)

                    # osekoil.g:598:32: ( objectDefinition )*
                    while stream_objectDefinition.hasNext():
                        self._adaptor.addChild(root_1, stream_objectDefinition.nextTree())


                    stream_objectDefinition.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



                if self._state.backtracking == 0:
                    pass
                           
                    odl = ApplicationDefinition.ObjectDefinitionList(self, myList)
                    retval.value = odl.run()




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "objectDefinitionList"


    class objectDefinition_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.objectDefinition_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "objectDefinition"
    # osekoil.g:602:1: objectDefinition returns [value] : objectName ( '{' parameterList '}' )? ( description )? SEMI -> ^( OBJECT_DEFINITION objectName ( parameterList )? ( description )? ) ;
    def objectDefinition(self, ):
        self.objectDefinition_stack.append(objectDefinition_scope())
        retval = self.objectDefinition_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal139 = None
        char_literal141 = None
        SEMI143 = None
        objectName138 = None

        parameterList140 = None

        description142 = None


        char_literal139_tree = None
        char_literal141_tree = None
        SEMI143_tree = None
        stream_LCURLY = RewriteRuleTokenStream(self._adaptor, "token LCURLY")
        stream_SEMI = RewriteRuleTokenStream(self._adaptor, "token SEMI")
        stream_RCURLY = RewriteRuleTokenStream(self._adaptor, "token RCURLY")
        stream_objectName = RewriteRuleSubtreeStream(self._adaptor, "rule objectName")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
        stream_parameterList = RewriteRuleSubtreeStream(self._adaptor, "rule parameterList")
        try:
            try:
                # osekoil.g:606:4: ( objectName ( '{' parameterList '}' )? ( description )? SEMI -> ^( OBJECT_DEFINITION objectName ( parameterList )? ( description )? ) )
                # osekoil.g:606:7: objectName ( '{' parameterList '}' )? ( description )? SEMI
                pass 
                self._state.following.append(self.FOLLOW_objectName_in_objectDefinition3033)
                objectName138 = self.objectName()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_objectName.add(objectName138.tree)


                if self._state.backtracking == 0:
                    pass
                    self.objectDefinition_stack[-1].objectName = ((objectName138 is not None) and [objectName138.name] or [None])[0] 



                # osekoil.g:607:7: ( '{' parameterList '}' )?
                alt69 = 2
                LA69_0 = self.input.LA(1)

                if (LA69_0 == LCURLY) :
                    alt69 = 1
                if alt69 == 1:
                    # osekoil.g:607:8: '{' parameterList '}'
                    pass 
                    char_literal139 = self.match(self.input, LCURLY, self.FOLLOW_LCURLY_in_objectDefinition3044) 
                    if self._state.backtracking == 0:
                        stream_LCURLY.add(char_literal139)


                    self._state.following.append(self.FOLLOW_parameterList_in_objectDefinition3046)
                    parameterList140 = self.parameterList()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_parameterList.add(parameterList140.tree)


                    char_literal141 = self.match(self.input, RCURLY, self.FOLLOW_RCURLY_in_objectDefinition3048) 
                    if self._state.backtracking == 0:
                        stream_RCURLY.add(char_literal141)





                # osekoil.g:607:32: ( description )?
                alt70 = 2
                LA70_0 = self.input.LA(1)

                if (LA70_0 == 78) :
                    alt70 = 1
                if alt70 == 1:
                    # osekoil.g:607:32: description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_objectDefinition3052)
                    description142 = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(description142.tree)





                SEMI143 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_objectDefinition3055) 
                if self._state.backtracking == 0:
                    stream_SEMI.add(SEMI143)


                if self._state.backtracking == 0:
                    pass
                       
                    obj = self.getObject(((objectName138 is not None) and [objectName138.objectType] or [None])[0], ((objectName138 is not None) and [objectName138.name] or [None])[0])
                    if obj:
                        retval.value = obj.appendValues(((objectName138 is not None) and [objectName138.objectType] or [None])[0], ((objectName138 is not None) and [objectName138.name] or [None])[0], ((parameterList140 is not None) and [parameterList140.value] or [None])[0], ((description142 is not None) and [description142.value] or [None])[0])
                    else:
                       retval.value = ApplicationDefinition.ObjectDefinition(self, ((objectName138 is not None) and [objectName138.objectType] or [None])[0], ((objectName138 is not None) and [objectName138.name] or [None])[0], ((parameterList140 is not None) and [parameterList140.value] or [None])[0], ((description142 is not None) and [description142.value] or [None])[0])
                       self.appendObject(((objectName138 is not None) and [objectName138.objectType] or [None])[0], ((objectName138 is not None) and [objectName138.name] or [None])[0], retval.value)
                       



                # AST Rewrite
                # elements: parameterList, objectName, description
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 616:7: -> ^( OBJECT_DEFINITION objectName ( parameterList )? ( description )? )
                    # osekoil.g:616:10: ^( OBJECT_DEFINITION objectName ( parameterList )? ( description )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(OBJECT_DEFINITION, "OBJECT_DEFINITION")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_objectName.nextTree())

                    # osekoil.g:616:41: ( parameterList )?
                    if stream_parameterList.hasNext():
                        self._adaptor.addChild(root_1, stream_parameterList.nextTree())


                    stream_parameterList.reset();

                    # osekoil.g:616:56: ( description )?
                    if stream_description.hasNext():
                        self._adaptor.addChild(root_1, stream_description.nextTree())


                    stream_description.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            self.objectDefinition_stack.pop()
            pass
        return retval

    # $ANTLR end "objectDefinition"


    class objectName_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.objectName_return, self).__init__()

            self.objectType = None
            self.name = None
            self.tree = None





    # $ANTLR start "objectName"
    # osekoil.g:620:1: objectName returns [objectType, name] : object ID -> ^( OBJECT_NAME object ID ) ;
    def objectName(self, ):
        retval = self.objectName_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID145 = None
        object144 = None


        ID145_tree = None
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_object = RewriteRuleSubtreeStream(self._adaptor, "rule object")
        try:
            try:
                # osekoil.g:621:4: ( object ID -> ^( OBJECT_NAME object ID ) )
                # osekoil.g:621:7: object ID
                pass 
                self._state.following.append(self.FOLLOW_object_in_objectName3100)
                object144 = self.object()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_object.add(object144.tree)


                ID145 = self.match(self.input, ID, self.FOLLOW_ID_in_objectName3102) 
                if self._state.backtracking == 0:
                    stream_ID.add(ID145)


                if self._state.backtracking == 0:
                    pass
                                    
                    # print ((object144 is not None) and [self.input.toString(object144.start,object144.stop)] or [None])[0], ID145.text
                    retval.objectType = ((object144 is not None) and [self.input.toString(object144.start,object144.stop)] or [None])[0]
                    retval.name = ID145.text
                       



                # AST Rewrite
                # elements: ID, object
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 626:4: -> ^( OBJECT_NAME object ID )
                    # osekoil.g:626:7: ^( OBJECT_NAME object ID )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(OBJECT_NAME, "OBJECT_NAME")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_object.nextTree())

                    self._adaptor.addChild(root_1, 
                    stream_ID.nextNode()
                    )

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "objectName"


    class parameterList_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.parameterList_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "parameterList"
    # osekoil.g:629:1: parameterList returns [value] : ( parameter )* -> ^( PARAMETER_LIST ( parameter )* ) ;
    def parameterList(self, ):
        retval = self.parameterList_return()
        retval.start = self.input.LT(1)


        root_0 = None

        parameter146 = None


        stream_parameter = RewriteRuleSubtreeStream(self._adaptor, "rule parameter")
              
        myList = ApplicationDefinition.ParameterList()

        try:
            try:
                # osekoil.g:636:4: ( ( parameter )* -> ^( PARAMETER_LIST ( parameter )* ) )
                # osekoil.g:636:7: ( parameter )*
                pass 
                # osekoil.g:636:7: ( parameter )*
                while True: #loop71
                    alt71 = 2
                    LA71_0 = self.input.LA(1)

                    if (LA71_0 == ID or LA71_0 == 79 or LA71_0 == 81 or LA71_0 == 85 or LA71_0 == 87 or LA71_0 == 91 or LA71_0 == 98 or LA71_0 == 100 or LA71_0 == 102 or LA71_0 == 104 or LA71_0 == 106 or LA71_0 == 110 or LA71_0 == 112 or LA71_0 == 115) :
                        alt71 = 1


                    if alt71 == 1:
                        # osekoil.g:636:8: parameter
                        pass 
                        self._state.following.append(self.FOLLOW_parameter_in_parameterList3147)
                        parameter146 = self.parameter()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_parameter.add(parameter146.tree)


                        if self._state.backtracking == 0:
                            pass
                            myList.append(((parameter146 is not None) and [parameter146.value] or [None])[0]) 




                    else:
                        break #loop71


                # AST Rewrite
                # elements: parameter
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 637:4: -> ^( PARAMETER_LIST ( parameter )* )
                    # osekoil.g:637:7: ^( PARAMETER_LIST ( parameter )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(PARAMETER_LIST, "PARAMETER_LIST")
                    , root_1)

                    # osekoil.g:637:24: ( parameter )*
                    while stream_parameter.hasNext():
                        self._adaptor.addChild(root_1, stream_parameter.nextTree())


                    stream_parameter.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



                if self._state.backtracking == 0:
                    pass
                           
                    retval.value = myList




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "parameterList"


    class parameter_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.parameter_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "parameter"
    # osekoil.g:640:1: parameter returns [value] : attributeName ASSIGN attributeValue ( description )? SEMI -> ^( PARAMETER attributeName attributeValue ( description )? ) ;
    def parameter(self, ):
        self.parameter_stack.append(parameter_scope())
        retval = self.parameter_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ASSIGN148 = None
        SEMI151 = None
        attributeName147 = None

        attributeValue149 = None

        description150 = None


        ASSIGN148_tree = None
        SEMI151_tree = None
        stream_SEMI = RewriteRuleTokenStream(self._adaptor, "token SEMI")
        stream_ASSIGN = RewriteRuleTokenStream(self._adaptor, "token ASSIGN")
        stream_attributeName = RewriteRuleSubtreeStream(self._adaptor, "rule attributeName")
        stream_attributeValue = RewriteRuleSubtreeStream(self._adaptor, "rule attributeValue")
        stream_description = RewriteRuleSubtreeStream(self._adaptor, "rule description")
        try:
            try:
                # osekoil.g:644:4: ( attributeName ASSIGN attributeValue ( description )? SEMI -> ^( PARAMETER attributeName attributeValue ( description )? ) )
                # osekoil.g:644:7: attributeName ASSIGN attributeValue ( description )? SEMI
                pass 
                self._state.following.append(self.FOLLOW_attributeName_in_parameter3187)
                attributeName147 = self.attributeName()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_attributeName.add(attributeName147.tree)


                ASSIGN148 = self.match(self.input, ASSIGN, self.FOLLOW_ASSIGN_in_parameter3189) 
                if self._state.backtracking == 0:
                    stream_ASSIGN.add(ASSIGN148)


                self._state.following.append(self.FOLLOW_attributeValue_in_parameter3191)
                attributeValue149 = self.attributeValue()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_attributeValue.add(attributeValue149.tree)


                # osekoil.g:644:43: ( description )?
                alt72 = 2
                LA72_0 = self.input.LA(1)

                if (LA72_0 == 78) :
                    alt72 = 1
                if alt72 == 1:
                    # osekoil.g:644:43: description
                    pass 
                    self._state.following.append(self.FOLLOW_description_in_parameter3193)
                    description150 = self.description()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_description.add(description150.tree)





                SEMI151 = self.match(self.input, SEMI, self.FOLLOW_SEMI_in_parameter3196) 
                if self._state.backtracking == 0:
                    stream_SEMI.add(SEMI151)


                if self._state.backtracking == 0:
                    pass
                       
                    self.parameter_stack[-1].attrName = ((attributeName147 is not None) and [self.input.toString(attributeName147.start,attributeName147.stop)] or [None])[0]
                    retval.value = ApplicationDefinition.Parameter(((attributeName147 is not None) and [self.input.toString(attributeName147.start,attributeName147.stop)] or [None])[0], ((attributeValue149 is not None) and [attributeValue149.value] or [None])[0], ((description150 is not None) and [description150.value] or [None])[0], self.objectDefinition_stack[-1].objectName)
                       



                # AST Rewrite
                # elements: attributeValue, attributeName, description
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 649:4: -> ^( PARAMETER attributeName attributeValue ( description )? )
                    # osekoil.g:649:7: ^( PARAMETER attributeName attributeValue ( description )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(PARAMETER, "PARAMETER")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_attributeName.nextTree())

                    self._adaptor.addChild(root_1, stream_attributeValue.nextTree())

                    # osekoil.g:649:48: ( description )?
                    if stream_description.hasNext():
                        self._adaptor.addChild(root_1, stream_description.nextTree())


                    stream_description.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            self.parameter_stack.pop()
            pass
        return retval

    # $ANTLR end "parameter"


    class attributeValue_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.attributeValue_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "attributeValue"
    # osekoil.g:652:1: attributeValue returns [value] : ( ID ( '{' parameterList '}' )? -> ^( ID_VALUE ID ( parameterList )? ) | boolean ( '{' parameterList '}' )? -> ^( BOOL_VALUE boolean ( parameterList )? ) | number -> ^( NUMBER_VALUE number ) | FLOAT -> ^( FLOAT_VALUE FLOAT ) | STRING -> ^( STRING_VALUE STRING ) | 'AUTO' -> ^( AUTO_VALUE ) );
    def attributeValue(self, ):
        retval = self.attributeValue_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID152 = None
        char_literal153 = None
        char_literal155 = None
        char_literal157 = None
        char_literal159 = None
        FLOAT161 = None
        STRING162 = None
        string_literal163 = None
        parameterList154 = None

        boolean156 = None

        parameterList158 = None

        number160 = None


        ID152_tree = None
        char_literal153_tree = None
        char_literal155_tree = None
        char_literal157_tree = None
        char_literal159_tree = None
        FLOAT161_tree = None
        STRING162_tree = None
        string_literal163_tree = None
        stream_LCURLY = RewriteRuleTokenStream(self._adaptor, "token LCURLY")
        stream_FLOAT = RewriteRuleTokenStream(self._adaptor, "token FLOAT")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_STRING = RewriteRuleTokenStream(self._adaptor, "token STRING")
        stream_RCURLY = RewriteRuleTokenStream(self._adaptor, "token RCURLY")
        stream_parameterList = RewriteRuleSubtreeStream(self._adaptor, "rule parameterList")
        stream_number = RewriteRuleSubtreeStream(self._adaptor, "rule number")
        stream_boolean = RewriteRuleSubtreeStream(self._adaptor, "rule boolean")
        try:
            try:
                # osekoil.g:656:4: ( ID ( '{' parameterList '}' )? -> ^( ID_VALUE ID ( parameterList )? ) | boolean ( '{' parameterList '}' )? -> ^( BOOL_VALUE boolean ( parameterList )? ) | number -> ^( NUMBER_VALUE number ) | FLOAT -> ^( FLOAT_VALUE FLOAT ) | STRING -> ^( STRING_VALUE STRING ) | 'AUTO' -> ^( AUTO_VALUE ) )
                alt75 = 6
                LA75 = self.input.LA(1)
                if LA75 == ID:
                    alt75 = 1
                elif LA75 == 93 or LA75 == 117:
                    alt75 = 2
                elif LA75 == HEX or LA75 == INT:
                    alt75 = 3
                elif LA75 == FLOAT:
                    alt75 = 4
                elif LA75 == STRING:
                    alt75 = 5
                elif LA75 == 83:
                    alt75 = 6
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 75, 0, self.input)

                    raise nvae


                if alt75 == 1:
                    # osekoil.g:656:7: ID ( '{' parameterList '}' )?
                    pass 
                    ID152 = self.match(self.input, ID, self.FOLLOW_ID_in_attributeValue3241) 
                    if self._state.backtracking == 0:
                        stream_ID.add(ID152)


                    # osekoil.g:656:10: ( '{' parameterList '}' )?
                    alt73 = 2
                    LA73_0 = self.input.LA(1)

                    if (LA73_0 == LCURLY) :
                        alt73 = 1
                    if alt73 == 1:
                        # osekoil.g:656:11: '{' parameterList '}'
                        pass 
                        char_literal153 = self.match(self.input, LCURLY, self.FOLLOW_LCURLY_in_attributeValue3244) 
                        if self._state.backtracking == 0:
                            stream_LCURLY.add(char_literal153)


                        self._state.following.append(self.FOLLOW_parameterList_in_attributeValue3246)
                        parameterList154 = self.parameterList()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_parameterList.add(parameterList154.tree)


                        char_literal155 = self.match(self.input, RCURLY, self.FOLLOW_RCURLY_in_attributeValue3248) 
                        if self._state.backtracking == 0:
                            stream_RCURLY.add(char_literal155)





                    if self._state.backtracking == 0:
                        pass
                           
                        retval.value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.ID_VALUE,
                            idValue = ID152.text, values = ((parameterList154 is not None) and [parameterList154.value] or [None])[0] or []
                        )
                           



                    # AST Rewrite
                    # elements: parameterList, ID
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 661:6: -> ^( ID_VALUE ID ( parameterList )? )
                        # osekoil.g:661:9: ^( ID_VALUE ID ( parameterList )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(ID_VALUE, "ID_VALUE")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )

                        # osekoil.g:661:23: ( parameterList )?
                        if stream_parameterList.hasNext():
                            self._adaptor.addChild(root_1, stream_parameterList.nextTree())


                        stream_parameterList.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt75 == 2:
                    # osekoil.g:662:7: boolean ( '{' parameterList '}' )?
                    pass 
                    self._state.following.append(self.FOLLOW_boolean_in_attributeValue3274)
                    boolean156 = self.boolean()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_boolean.add(boolean156.tree)


                    # osekoil.g:662:15: ( '{' parameterList '}' )?
                    alt74 = 2
                    LA74_0 = self.input.LA(1)

                    if (LA74_0 == LCURLY) :
                        alt74 = 1
                    if alt74 == 1:
                        # osekoil.g:662:16: '{' parameterList '}'
                        pass 
                        char_literal157 = self.match(self.input, LCURLY, self.FOLLOW_LCURLY_in_attributeValue3277) 
                        if self._state.backtracking == 0:
                            stream_LCURLY.add(char_literal157)


                        self._state.following.append(self.FOLLOW_parameterList_in_attributeValue3279)
                        parameterList158 = self.parameterList()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_parameterList.add(parameterList158.tree)


                        char_literal159 = self.match(self.input, RCURLY, self.FOLLOW_RCURLY_in_attributeValue3281) 
                        if self._state.backtracking == 0:
                            stream_RCURLY.add(char_literal159)





                    if self._state.backtracking == 0:
                        pass
                           
                        retval.value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.BOOL_VALUE,
                            booleanValue = ((boolean156 is not None) and [boolean156.value] or [None])[0], values = ((parameterList158 is not None) and [parameterList158.value] or [None])[0] or []
                        )
                           



                    # AST Rewrite
                    # elements: boolean, parameterList
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 667:6: -> ^( BOOL_VALUE boolean ( parameterList )? )
                        # osekoil.g:667:9: ^( BOOL_VALUE boolean ( parameterList )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(BOOL_VALUE, "BOOL_VALUE")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_boolean.nextTree())

                        # osekoil.g:667:30: ( parameterList )?
                        if stream_parameterList.hasNext():
                            self._adaptor.addChild(root_1, stream_parameterList.nextTree())


                        stream_parameterList.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt75 == 3:
                    # osekoil.g:668:7: number
                    pass 
                    self._state.following.append(self.FOLLOW_number_in_attributeValue3307)
                    number160 = self.number()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_number.add(number160.tree)


                    if self._state.backtracking == 0:
                        pass
                           
                        retval.value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.NUMBER_VALUE,
                            number = ((number160 is not None) and [number160.value] or [None])[0]
                        )
                           



                    # AST Rewrite
                    # elements: number
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 673:6: -> ^( NUMBER_VALUE number )
                        # osekoil.g:673:9: ^( NUMBER_VALUE number )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(NUMBER_VALUE, "NUMBER_VALUE")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_number.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt75 == 4:
                    # osekoil.g:674:7: FLOAT
                    pass 
                    FLOAT161 = self.match(self.input, FLOAT, self.FOLLOW_FLOAT_in_attributeValue3328) 
                    if self._state.backtracking == 0:
                        stream_FLOAT.add(FLOAT161)


                    if self._state.backtracking == 0:
                        pass
                           
                        retval.value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.FLOAT_VALUE,
                            number = float(FLOAT161.text)
                        )

                           



                    # AST Rewrite
                    # elements: FLOAT
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 680:6: -> ^( FLOAT_VALUE FLOAT )
                        # osekoil.g:680:9: ^( FLOAT_VALUE FLOAT )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(FLOAT_VALUE, "FLOAT_VALUE")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_FLOAT.nextNode()
                        )

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt75 == 5:
                    # osekoil.g:681:7: STRING
                    pass 
                    STRING162 = self.match(self.input, STRING, self.FOLLOW_STRING_in_attributeValue3349) 
                    if self._state.backtracking == 0:
                        stream_STRING.add(STRING162)


                    if self._state.backtracking == 0:
                        pass
                           
                        retval.value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.STRING_VALUE,
                            stringValue = STRING162.text.strip('"')
                        )
                           



                    # AST Rewrite
                    # elements: STRING
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 686:6: -> ^( STRING_VALUE STRING )
                        # osekoil.g:686:9: ^( STRING_VALUE STRING )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(STRING_VALUE, "STRING_VALUE")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_STRING.nextNode()
                        )

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt75 == 6:
                    # osekoil.g:687:7: 'AUTO'
                    pass 
                    string_literal163 = self.match(self.input, 83, self.FOLLOW_83_in_attributeValue3370) 
                    if self._state.backtracking == 0:
                        stream_83.add(string_literal163)


                    if self._state.backtracking == 0:
                        pass
                           
                        retval.value = ApplicationDefinition.AttributeValueContainter(ApplicationDefinition.AttributeValueContainter.AUTO_VALUE)
                           



                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 690:6: -> ^( AUTO_VALUE )
                        # osekoil.g:690:9: ^( AUTO_VALUE )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(AUTO_VALUE, "AUTO_VALUE")
                        , root_1)

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)


                if self._state.backtracking == 0:
                    pass
                           





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "attributeValue"


    class description_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.description_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "description"
    # osekoil.g:694:1: description returns [value] : ( ':' STRING ) -> ^( DESCRIPTION STRING ) ;
    def description(self, ):
        retval = self.description_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal164 = None
        STRING165 = None

        char_literal164_tree = None
        STRING165_tree = None
        stream_78 = RewriteRuleTokenStream(self._adaptor, "token 78")
        stream_STRING = RewriteRuleTokenStream(self._adaptor, "token STRING")

        try:
            try:
                # osekoil.g:695:4: ( ( ':' STRING ) -> ^( DESCRIPTION STRING ) )
                # osekoil.g:695:7: ( ':' STRING )
                pass 
                # osekoil.g:695:7: ( ':' STRING )
                # osekoil.g:695:8: ':' STRING
                pass 
                char_literal164 = self.match(self.input, 78, self.FOLLOW_78_in_description3402) 
                if self._state.backtracking == 0:
                    stream_78.add(char_literal164)


                STRING165 = self.match(self.input, STRING, self.FOLLOW_STRING_in_description3404) 
                if self._state.backtracking == 0:
                    stream_STRING.add(STRING165)





                if self._state.backtracking == 0:
                    pass
                    retval.value = STRING165.text.strip('"') 



                # AST Rewrite
                # elements: STRING
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 697:4: -> ^( DESCRIPTION STRING )
                    # osekoil.g:697:7: ^( DESCRIPTION STRING )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(DESCRIPTION, "DESCRIPTION")
                    , root_1)

                    self._adaptor.addChild(root_1, 
                    stream_STRING.nextNode()
                    )

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "description"


    class number_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.number_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "number"
    # osekoil.g:700:1: number returns [value] : (id= INT |id= HEX -> ^( NUMBER $id) );
    def number(self, ):
        retval = self.number_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None

        id_tree = None
        stream_HEX = RewriteRuleTokenStream(self._adaptor, "token HEX")

        try:
            try:
                # osekoil.g:701:4: (id= INT |id= HEX -> ^( NUMBER $id) )
                alt76 = 2
                LA76_0 = self.input.LA(1)

                if (LA76_0 == INT) :
                    alt76 = 1
                elif (LA76_0 == HEX) :
                    alt76 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 76, 0, self.input)

                    raise nvae


                if alt76 == 1:
                    # osekoil.g:701:6: id= INT
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, INT, self.FOLLOW_INT_in_number3443)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)



                    if self._state.backtracking == 0:
                        pass
                        retval.value = long(id.text) 




                elif alt76 == 2:
                    # osekoil.g:702:6: id= HEX
                    pass 
                    id = self.match(self.input, HEX, self.FOLLOW_HEX_in_number3456) 
                    if self._state.backtracking == 0:
                        stream_HEX.add(id)


                    if self._state.backtracking == 0:
                        pass
                        retval.value = long(id.text[2 : ], 16) 



                    # AST Rewrite
                    # elements: id
                    # token labels: id
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        stream_id = RewriteRuleTokenStream(self._adaptor, "token id", id)
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 703:4: -> ^( NUMBER $id)
                        # osekoil.g:703:7: ^( NUMBER $id)
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(NUMBER, "NUMBER")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_id.nextNode())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "number"


    class range_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.range_return, self).__init__()

            self.tree = None





    # $ANTLR start "range"
    # osekoil.g:706:1: range : ( '..' )=> '..' ;
    def range(self, ):
        retval = self.range_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal166 = None

        string_literal166_tree = None

        try:
            try:
                # osekoil.g:707:4: ( ( '..' )=> '..' )
                # osekoil.g:707:7: ( '..' )=> '..'
                pass 
                root_0 = self._adaptor.nil()


                string_literal166 = self.match(self.input, 77, self.FOLLOW_77_in_range3491)
                if self._state.backtracking == 0:
                    string_literal166_tree = self._adaptor.createWithPayload(string_literal166)
                    self._adaptor.addChild(root_0, string_literal166_tree)





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "range"


    class boolean_return(ParserRuleReturnScope):
        def __init__(self):
            super(osekoilParser.boolean_return, self).__init__()

            self.value = None
            self.tree = None





    # $ANTLR start "boolean"
    # osekoil.g:710:1: boolean returns [value] : (id= 'TRUE' |id= 'FALSE' );
    def boolean(self, ):
        retval = self.boolean_return()
        retval.start = self.input.LT(1)


        root_0 = None

        id = None

        id_tree = None

        try:
            try:
                # osekoil.g:711:4: (id= 'TRUE' |id= 'FALSE' )
                alt77 = 2
                LA77_0 = self.input.LA(1)

                if (LA77_0 == 117) :
                    alt77 = 1
                elif (LA77_0 == 93) :
                    alt77 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 77, 0, self.input)

                    raise nvae


                if alt77 == 1:
                    # osekoil.g:711:6: id= 'TRUE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 117, self.FOLLOW_117_in_boolean3513)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)



                    if self._state.backtracking == 0:
                        pass
                        retval.value = True  




                elif alt77 == 2:
                    # osekoil.g:712:6: id= 'FALSE'
                    pass 
                    root_0 = self._adaptor.nil()


                    id = self.match(self.input, 93, self.FOLLOW_93_in_boolean3527)
                    if self._state.backtracking == 0:
                        id_tree = self._adaptor.createWithPayload(id)
                        self._adaptor.addChild(root_0, id_tree)



                    if self._state.backtracking == 0:
                        pass
                        retval.value = False 




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "boolean"



 

    FOLLOW_oilVersion_in_file387 = frozenset([95])
    FOLLOW_implementationDefinition_in_file389 = frozenset([89])
    FOLLOW_applicationDefinition_in_file391 = frozenset([1])
    FOLLOW_109_in_oilVersion427 = frozenset([5])
    FOLLOW_ASSIGN_in_oilVersion429 = frozenset([71])
    FOLLOW_STRING_in_oilVersion431 = frozenset([68, 78])
    FOLLOW_description_in_oilVersion433 = frozenset([68])
    FOLLOW_SEMI_in_oilVersion436 = frozenset([1])
    FOLLOW_95_in_implementationDefinition474 = frozenset([33])
    FOLLOW_ID_in_implementationDefinition476 = frozenset([46])
    FOLLOW_LCURLY_in_implementationDefinition478 = frozenset([65, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_implementationSpecList_in_implementationDefinition480 = frozenset([65])
    FOLLOW_RCURLY_in_implementationDefinition482 = frozenset([68, 78])
    FOLLOW_description_in_implementationDefinition484 = frozenset([68])
    FOLLOW_SEMI_in_implementationDefinition487 = frozenset([1])
    FOLLOW_implementationSpec_in_implementationSpecList541 = frozenset([1, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_object_in_implementationSpec587 = frozenset([46])
    FOLLOW_LCURLY_in_implementationSpec591 = frozenset([65, 80, 82, 84, 86, 88, 90, 92, 94, 96, 97, 99, 101, 103, 105, 107, 111, 113, 114, 116, 118, 119])
    FOLLOW_implementationList_in_implementationSpec593 = frozenset([65])
    FOLLOW_RCURLY_in_implementationSpec595 = frozenset([68, 78])
    FOLLOW_description_in_implementationSpec597 = frozenset([68])
    FOLLOW_SEMI_in_implementationSpec600 = frozenset([1])
    FOLLOW_110_in_object644 = frozenset([1])
    FOLLOW_115_in_object656 = frozenset([1])
    FOLLOW_87_in_object668 = frozenset([1])
    FOLLOW_79_in_object680 = frozenset([1])
    FOLLOW_112_in_object692 = frozenset([1])
    FOLLOW_91_in_object704 = frozenset([1])
    FOLLOW_100_in_object716 = frozenset([1])
    FOLLOW_102_in_object728 = frozenset([1])
    FOLLOW_85_in_object740 = frozenset([1])
    FOLLOW_106_in_object752 = frozenset([1])
    FOLLOW_81_in_object764 = frozenset([1])
    FOLLOW_98_in_object776 = frozenset([1])
    FOLLOW_104_in_object788 = frozenset([1])
    FOLLOW_implementationDef_in_implementationList834 = frozenset([1, 80, 82, 84, 86, 88, 90, 92, 94, 96, 97, 99, 101, 103, 105, 107, 111, 113, 114, 116, 118, 119])
    FOLLOW_implAttrDef_in_implementationDef879 = frozenset([1])
    FOLLOW_implRefDef_in_implementationDef906 = frozenset([1])
    FOLLOW_118_in_implAttrDef957 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115, 120])
    FOLLOW_autoSpecifier_in_implAttrDef959 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_numberRange_in_implAttrDef962 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_implAttrDef965 = frozenset([5, 45, 68, 78])
    FOLLOW_multipleSpecifier_in_implAttrDef967 = frozenset([5, 68, 78])
    FOLLOW_defaultNumber_in_implAttrDef970 = frozenset([68, 78])
    FOLLOW_description_in_implAttrDef973 = frozenset([68])
    FOLLOW_SEMI_in_implAttrDef976 = frozenset([1])
    FOLLOW_96_in_implAttrDef1023 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115, 120])
    FOLLOW_autoSpecifier_in_implAttrDef1029 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_numberRange_in_implAttrDef1032 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_implAttrDef1035 = frozenset([5, 45, 68, 78])
    FOLLOW_multipleSpecifier_in_implAttrDef1037 = frozenset([5, 68, 78])
    FOLLOW_defaultNumber_in_implAttrDef1040 = frozenset([68, 78])
    FOLLOW_description_in_implAttrDef1043 = frozenset([68])
    FOLLOW_SEMI_in_implAttrDef1046 = frozenset([1])
    FOLLOW_119_in_implAttrDef1093 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115, 120])
    FOLLOW_autoSpecifier_in_implAttrDef1095 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_numberRange_in_implAttrDef1098 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_implAttrDef1101 = frozenset([5, 45, 68, 78])
    FOLLOW_multipleSpecifier_in_implAttrDef1103 = frozenset([5, 68, 78])
    FOLLOW_defaultNumber_in_implAttrDef1106 = frozenset([68, 78])
    FOLLOW_description_in_implAttrDef1109 = frozenset([68])
    FOLLOW_SEMI_in_implAttrDef1112 = frozenset([1])
    FOLLOW_97_in_implAttrDef1159 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115, 120])
    FOLLOW_autoSpecifier_in_implAttrDef1165 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_numberRange_in_implAttrDef1168 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_implAttrDef1171 = frozenset([5, 45, 68, 78])
    FOLLOW_multipleSpecifier_in_implAttrDef1173 = frozenset([5, 68, 78])
    FOLLOW_defaultNumber_in_implAttrDef1176 = frozenset([68, 78])
    FOLLOW_description_in_implAttrDef1179 = frozenset([68])
    FOLLOW_SEMI_in_implAttrDef1182 = frozenset([1])
    FOLLOW_94_in_implAttrDef1229 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115, 120])
    FOLLOW_autoSpecifier_in_implAttrDef1235 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_floatRange_in_implAttrDef1238 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_implAttrDef1241 = frozenset([5, 45, 68, 78])
    FOLLOW_multipleSpecifier_in_implAttrDef1243 = frozenset([5, 68, 78])
    FOLLOW_defaultFloat_in_implAttrDef1246 = frozenset([68, 78])
    FOLLOW_description_in_implAttrDef1249 = frozenset([68])
    FOLLOW_SEMI_in_implAttrDef1252 = frozenset([1])
    FOLLOW_90_in_implAttrDef1299 = frozenset([45, 120])
    FOLLOW_autoSpecifier_in_implAttrDef1306 = frozenset([45])
    FOLLOW_enumeration_in_implAttrDef1309 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_implAttrDef1311 = frozenset([5, 45, 68, 78])
    FOLLOW_multipleSpecifier_in_implAttrDef1313 = frozenset([5, 68, 78])
    FOLLOW_defaultName_in_implAttrDef1316 = frozenset([68, 78])
    FOLLOW_description_in_implAttrDef1319 = frozenset([68])
    FOLLOW_SEMI_in_implAttrDef1322 = frozenset([1])
    FOLLOW_114_in_implAttrDef1368 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115, 120])
    FOLLOW_autoSpecifier_in_implAttrDef1370 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_implAttrDef1373 = frozenset([5, 45, 68, 78])
    FOLLOW_multipleSpecifier_in_implAttrDef1375 = frozenset([5, 68, 78])
    FOLLOW_defaultString_in_implAttrDef1378 = frozenset([68, 78])
    FOLLOW_description_in_implAttrDef1381 = frozenset([68])
    FOLLOW_SEMI_in_implAttrDef1384 = frozenset([1])
    FOLLOW_84_in_implAttrDef1428 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115, 120])
    FOLLOW_autoSpecifier_in_implAttrDef1432 = frozenset([33, 45, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_boolValues_in_implAttrDef1435 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_implAttrDef1438 = frozenset([5, 45, 68, 78])
    FOLLOW_multipleSpecifier_in_implAttrDef1440 = frozenset([5, 68, 78])
    FOLLOW_defaultBool_in_implAttrDef1443 = frozenset([68, 78])
    FOLLOW_description_in_implAttrDef1446 = frozenset([68])
    FOLLOW_SEMI_in_implAttrDef1449 = frozenset([1])
    FOLLOW_ID_in_attributeName1504 = frozenset([1])
    FOLLOW_object_in_attributeName1524 = frozenset([1])
    FOLLOW_LBRACK_in_numberRange1552 = frozenset([31, 44])
    FOLLOW_numberList_in_numberRange1555 = frozenset([64])
    FOLLOW_number_in_numberRange1584 = frozenset([77])
    FOLLOW_range_in_numberRange1591 = frozenset([31, 44])
    FOLLOW_number_in_numberRange1597 = frozenset([64])
    FOLLOW_RBRACK_in_numberRange1625 = frozenset([1])
    FOLLOW_number_in_numberList1654 = frozenset([1, 76])
    FOLLOW_76_in_numberList1657 = frozenset([31, 44])
    FOLLOW_number_in_numberList1663 = frozenset([1, 76])
    FOLLOW_LBRACK_in_multipleSpecifier1699 = frozenset([64])
    FOLLOW_RBRACK_in_multipleSpecifier1701 = frozenset([1])
    FOLLOW_LBRACK_in_floatRange1728 = frozenset([27])
    FOLLOW_FLOAT_in_floatRange1735 = frozenset([77])
    FOLLOW_range_in_floatRange1739 = frozenset([27])
    FOLLOW_FLOAT_in_floatRange1745 = frozenset([64])
    FOLLOW_RBRACK_in_floatRange1750 = frozenset([1])
    FOLLOW_LBRACK_in_enumeration1785 = frozenset([33])
    FOLLOW_enumeratorList_in_enumeration1787 = frozenset([64])
    FOLLOW_RBRACK_in_enumeration1789 = frozenset([1])
    FOLLOW_enumerator_in_enumeratorList1838 = frozenset([1, 76])
    FOLLOW_76_in_enumeratorList1843 = frozenset([33])
    FOLLOW_enumerator_in_enumeratorList1849 = frozenset([1, 76])
    FOLLOW_ID_in_enumerator1894 = frozenset([1, 46, 78])
    FOLLOW_implParameterList_in_enumerator1896 = frozenset([1, 78])
    FOLLOW_description_in_enumerator1899 = frozenset([1])
    FOLLOW_LBRACK_in_boolValues1952 = frozenset([117])
    FOLLOW_117_in_boolValues1954 = frozenset([46, 76, 78])
    FOLLOW_implParameterList_in_boolValues1960 = frozenset([76, 78])
    FOLLOW_description_in_boolValues1967 = frozenset([76])
    FOLLOW_76_in_boolValues1970 = frozenset([93])
    FOLLOW_93_in_boolValues1972 = frozenset([46, 64, 78])
    FOLLOW_implParameterList_in_boolValues1978 = frozenset([64, 78])
    FOLLOW_description_in_boolValues1985 = frozenset([64])
    FOLLOW_RBRACK_in_boolValues1988 = frozenset([1])
    FOLLOW_120_in_autoSpecifier2043 = frozenset([1])
    FOLLOW_ASSIGN_in_defaultNumber2064 = frozenset([31, 44, 83, 108])
    FOLLOW_number_in_defaultNumber2075 = frozenset([1])
    FOLLOW_108_in_defaultNumber2098 = frozenset([1])
    FOLLOW_83_in_defaultNumber2121 = frozenset([1])
    FOLLOW_ASSIGN_in_defaultFloat2165 = frozenset([27, 83, 108])
    FOLLOW_float_in_defaultFloat2176 = frozenset([1])
    FOLLOW_108_in_defaultFloat2199 = frozenset([1])
    FOLLOW_83_in_defaultFloat2222 = frozenset([1])
    FOLLOW_FLOAT_in_float2261 = frozenset([1])
    FOLLOW_ASSIGN_in_defaultName2292 = frozenset([33, 83, 108])
    FOLLOW_ID_in_defaultName2303 = frozenset([1])
    FOLLOW_108_in_defaultName2314 = frozenset([1])
    FOLLOW_83_in_defaultName2325 = frozenset([1])
    FOLLOW_ASSIGN_in_defaultString2372 = frozenset([71, 83, 108])
    FOLLOW_STRING_in_defaultString2383 = frozenset([1])
    FOLLOW_108_in_defaultString2394 = frozenset([1])
    FOLLOW_83_in_defaultString2405 = frozenset([1])
    FOLLOW_ASSIGN_in_defaultBool2452 = frozenset([83, 93, 108, 117])
    FOLLOW_boolean_in_defaultBool2459 = frozenset([1])
    FOLLOW_108_in_defaultBool2486 = frozenset([1])
    FOLLOW_83_in_defaultBool2506 = frozenset([1])
    FOLLOW_LCURLY_in_implParameterList2547 = frozenset([65, 80, 82, 84, 86, 88, 90, 92, 94, 96, 97, 99, 101, 103, 105, 107, 111, 113, 114, 116, 118, 119])
    FOLLOW_implDefList_in_implParameterList2549 = frozenset([65])
    FOLLOW_RCURLY_in_implParameterList2556 = frozenset([1])
    FOLLOW_implementationDef_in_implDefList2602 = frozenset([1, 80, 82, 84, 86, 88, 90, 92, 94, 96, 97, 99, 101, 103, 105, 107, 111, 113, 114, 116, 118, 119])
    FOLLOW_objectRefType_in_implRefDef2644 = frozenset([33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_referenceName_in_implRefDef2646 = frozenset([45, 68, 78])
    FOLLOW_multipleSpecifier_in_implRefDef2648 = frozenset([68, 78])
    FOLLOW_description_in_implRefDef2651 = frozenset([68])
    FOLLOW_SEMI_in_implRefDef2654 = frozenset([1])
    FOLLOW_111_in_objectRefType2701 = frozenset([1])
    FOLLOW_116_in_objectRefType2713 = frozenset([1])
    FOLLOW_88_in_objectRefType2725 = frozenset([1])
    FOLLOW_80_in_objectRefType2737 = frozenset([1])
    FOLLOW_113_in_objectRefType2749 = frozenset([1])
    FOLLOW_92_in_objectRefType2761 = frozenset([1])
    FOLLOW_101_in_objectRefType2773 = frozenset([1])
    FOLLOW_103_in_objectRefType2785 = frozenset([1])
    FOLLOW_86_in_objectRefType2797 = frozenset([1])
    FOLLOW_107_in_objectRefType2809 = frozenset([1])
    FOLLOW_82_in_objectRefType2821 = frozenset([1])
    FOLLOW_99_in_objectRefType2833 = frozenset([1])
    FOLLOW_105_in_objectRefType2845 = frozenset([1])
    FOLLOW_ID_in_referenceName2873 = frozenset([1])
    FOLLOW_object_in_referenceName2889 = frozenset([1])
    FOLLOW_89_in_applicationDefinition2923 = frozenset([33])
    FOLLOW_ID_in_applicationDefinition2925 = frozenset([46])
    FOLLOW_LCURLY_in_applicationDefinition2927 = frozenset([65, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_objectDefinitionList_in_applicationDefinition2929 = frozenset([65])
    FOLLOW_RCURLY_in_applicationDefinition2931 = frozenset([68, 78])
    FOLLOW_description_in_applicationDefinition2933 = frozenset([68])
    FOLLOW_SEMI_in_applicationDefinition2936 = frozenset([1])
    FOLLOW_objectDefinition_in_objectDefinitionList2991 = frozenset([1, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_objectName_in_objectDefinition3033 = frozenset([46, 68, 78])
    FOLLOW_LCURLY_in_objectDefinition3044 = frozenset([33, 65, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_parameterList_in_objectDefinition3046 = frozenset([65])
    FOLLOW_RCURLY_in_objectDefinition3048 = frozenset([68, 78])
    FOLLOW_description_in_objectDefinition3052 = frozenset([68])
    FOLLOW_SEMI_in_objectDefinition3055 = frozenset([1])
    FOLLOW_object_in_objectName3100 = frozenset([33])
    FOLLOW_ID_in_objectName3102 = frozenset([1])
    FOLLOW_parameter_in_parameterList3147 = frozenset([1, 33, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_attributeName_in_parameter3187 = frozenset([5])
    FOLLOW_ASSIGN_in_parameter3189 = frozenset([27, 31, 33, 44, 71, 83, 93, 117])
    FOLLOW_attributeValue_in_parameter3191 = frozenset([68, 78])
    FOLLOW_description_in_parameter3193 = frozenset([68])
    FOLLOW_SEMI_in_parameter3196 = frozenset([1])
    FOLLOW_ID_in_attributeValue3241 = frozenset([1, 46])
    FOLLOW_LCURLY_in_attributeValue3244 = frozenset([33, 65, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_parameterList_in_attributeValue3246 = frozenset([65])
    FOLLOW_RCURLY_in_attributeValue3248 = frozenset([1])
    FOLLOW_boolean_in_attributeValue3274 = frozenset([1, 46])
    FOLLOW_LCURLY_in_attributeValue3277 = frozenset([33, 65, 79, 81, 85, 87, 91, 98, 100, 102, 104, 106, 110, 112, 115])
    FOLLOW_parameterList_in_attributeValue3279 = frozenset([65])
    FOLLOW_RCURLY_in_attributeValue3281 = frozenset([1])
    FOLLOW_number_in_attributeValue3307 = frozenset([1])
    FOLLOW_FLOAT_in_attributeValue3328 = frozenset([1])
    FOLLOW_STRING_in_attributeValue3349 = frozenset([1])
    FOLLOW_83_in_attributeValue3370 = frozenset([1])
    FOLLOW_78_in_description3402 = frozenset([71])
    FOLLOW_STRING_in_description3404 = frozenset([1])
    FOLLOW_INT_in_number3443 = frozenset([1])
    FOLLOW_HEX_in_number3456 = frozenset([1])
    FOLLOW_77_in_range3491 = frozenset([1])
    FOLLOW_117_in_boolean3513 = frozenset([1])
    FOLLOW_93_in_boolean3527 = frozenset([1])



def main(argv, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
    from antlr3.main import ParserMain
    main = ParserMain("osekoilLexer", osekoilParser)

    main.stdin = stdin
    main.stdout = stdout
    main.stderr = stderr
    main.execute(argv)



if __name__ == '__main__':
    main(sys.argv)
