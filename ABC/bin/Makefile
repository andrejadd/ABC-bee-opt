ABC_PATH    = abc/
PSO_PATH    = pso/
GLOBAL_PATH = global/
DE_PATH = de/

GLOBALSOURCES  = global/ModelInterface.java 

DEBUG = -g -Xlint:unchecked

ModelRunner.class: 
	javac ModelRunner.java global/Solution.java

ab: ModelRunner.class
	javac -sourcepath . -cp . global/ModelInterface.java  ${ABC_PATH}ABCModel.java ${ABC_PATH}EmployedBee.java ${ABC_PATH}OnlookerBee.java global/TestFunction.java global/Solution.java global/Agent.java ${ABC_PATH}ABCBlackboard.java ModelRunner.java global/Blackboard.java 

ps:
	javac ${DEBUG} -sourcepath . -cp . ${GLOBALSOURCES} ${PSO_PATH}PSOModel.java ${PSO_PATH}PSOAgent.java 

dee:
	javac ${DEBUG} -sourcepath . -cp . ${GLOBALSOURCES} ${DE_PATH}*.java

model: dee
	javac ${DEBUG} -cp . ModelRunner.java

run: model
	java ModelRunner

clean: 
	rm *.class
	rm abc/*.class
	rm pso/*.class
	rm global/*.class

debug: model
	gcj -g --main=ModelRunner -o ModelRunner ModelRunner.class ${GLOBAL_PATH}*.class ${DE_PATH}*.class
	gdb ModelRunner
