
try:
  import ConfigParser
except ImportError:
  import configpaser
#from collections import OrderedDict
import kosek.ORTICfg as cfg
import cStringIO
import sys

FNAME ="ORTIConf.ini"

def main():
  outf = file(FNAME, "wt")
  print >> outf, ";"
  print >> outf, "; AUTOMATICALLY GENERATED FILE -- DO NOT MODIFY!"
  print >> outf,";"
  for item in [x for x in dir(cfg) if x.isupper()]:
    print >>outf, "[%s]" % item
    for (key, value) in getattr(cfg, item):
      print >> outf, "%-27s = %s" % (key, value)
    print >> outf, ""
    #cfp.write(outf)
  outf.close()
  

class CfgParser(ConfigParser.ConfigParser):
  def optionxform(self, opt):
    return opt	# Preserve case-sensitivity.


if __name__ == '__main__':
  cfp = CfgParser()
  configuration = dict()
  try:
    fl = cfp.read(FNAME)
    print fl
  except:
    print sys.exc_info()
  for section in cfp.sections():
    configuration[section] = dict()
    configuration[section].update(cfp.items(section))
  print configuration
  main()