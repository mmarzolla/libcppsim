bin_PROGRAMS=dicegame permutations checkboard rantest cor1 pcheckboard mm1 handleTest trivialTest quantiletest foo
EXTRA_DIST = qn.cc

AM_LDFLAGS = @LDFLAGS@ -static

INCLUDES = -I$(top_srcdir)/src

LDADD = $(top_builddir)/src/libcppsim.a

dicegame_SOURCES=dicegame.cc

permutations_SOURCES=permutations.cc

checkboard_SOURCES=checkboard.cc

rantest_SOURCES=rantest.cc

cor1_SOURCES=cor1.cc

pcheckboard_SOURCES=pcheckboard.cc

mm1_SOURCES=mm1.cc

handleTest_SOURCES=handleTest.cc

trivialTest_SOURCES=trivialTest.cc

quantiletest_SOURCES=quantiletest.cc

foo_SOURCES=foo.cc

TESTS = cor1 rantest checkboard permutations pcheckboard mm1 handleTest trivialTest quantiletest foo 
