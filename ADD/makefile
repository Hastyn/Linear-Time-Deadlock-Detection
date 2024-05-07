SOLUTION = SolutionRg
NETWORK = rl-toy
CC = g++
CFLAGS ?=-g -w -c -Wall -Wno-unused-value
SRCDIR = src/
INCDIR = include/
OBJDIR = obj/
TRAINPS_SRC = ps/
TRAINPG_SRC = pg/
SOLS = $(OBJDIR)Solution.o $(OBJDIR)SolutionRg.o $(OBJDIR)SolutionR0.o $(OBJDIR)SolutionRc.o
OBJECTS = $(OBJDIR)Simulator.o $(OBJDIR)Scheduler_Custom.o $(OBJDIR)Scheduler_PolicyImprovement.o $(OBJDIR)Scheduler_OptimizedPolicyImprovement.o $(OBJDIR)Scheduler.o $(OBJDIR)Network.o $(OBJDIR)Train.o $(OBJDIR)Journey.o $(OBJDIR)Section.o $(OBJDIR)Station.o $(OBJDIR)Resource.o $(OBJDIR)TrainEvent.o $(SOLS)
TARGET=$(OBJDIR)simulate 


$(TARGET): $(OBJDIR)run.o $(OBJECTS)
	$(CC) -g -w -Wall -Wno-unused-value $(OBJDIR)run.o $(OBJECTS) -o $(TARGET)

$(OBJDIR)run.o: $(SRCDIR)run.cpp $(INCDIR)Simulator.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)run.cpp -o $@

$(OBJDIR)Simulator.o: $(SRCDIR)Simulator.cpp $(INCDIR)Simulator.h $(INCDIR)Scheduler.h $(INCDIR)Network.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)Simulator.cpp -o $@

$(OBJDIR)Scheduler_Custom.o: $(SRCDIR)Scheduler_Custom.cpp $(INCDIR)Scheduler_Custom.h $(INCDIR)Scheduler.h $(INCDIR)Network.h $(INCDIR)Train.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)Scheduler_Custom.cpp -o $@

$(OBJDIR)Scheduler_PolicyImprovement.o: $(SRCDIR)Scheduler_PolicyImprovement.cpp $(INCDIR)Scheduler_PolicyImprovement.h $(INCDIR)Scheduler.h $(INCDIR)Network.h $(INCDIR)Train.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)Scheduler_PolicyImprovement.cpp -o $@

$(OBJDIR)Scheduler_OptimizedPolicyImprovement.o: $(SRCDIR)Scheduler_OptimizedPolicyImprovement.cpp $(INCDIR)Scheduler_OptimizedPolicyImprovement.h $(INCDIR)Scheduler.h $(INCDIR)Network.h $(INCDIR)Train.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)Scheduler_OptimizedPolicyImprovement.cpp -o $@

$(OBJDIR)Scheduler.o: $(SRCDIR)Scheduler.cpp $(INCDIR)Scheduler.h $(INCDIR)Network.h $(INCDIR)Train.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)Scheduler.cpp -o $@

$(OBJDIR)Network.o: $(SRCDIR)Network.cpp $(INCDIR)Network.h $(INCDIR)Train.h $(INCDIR)Section.h $(INCDIR)Station.h
	$(CC) $(CFLAGS) $(SRCDIR)Network.cpp -o $@

$(OBJDIR)Train.o: $(SRCDIR)Train.cpp $(INCDIR)Train.h $(INCDIR)Journey.h $(INCDIR)Resource.h
	$(CC) $(CFLAGS) $(SRCDIR)Train.cpp -o $@

$(OBJDIR)Journey.o: $(SRCDIR)Journey.cpp $(INCDIR)Journey.h
	$(CC) $(CFLAGS) $(SRCDIR)Journey.cpp -o $@

$(OBJDIR)Section.o: $(SRCDIR)Section.cpp $(INCDIR)Section.h $(INCDIR)Resource.h
	$(CC) $(CFLAGS) $(SRCDIR)Section.cpp -o $@

$(OBJDIR)Station.o: $(SRCDIR)Station.cpp $(INCDIR)Station.h $(INCDIR)Resource.h
	$(CC) $(CFLAGS) $(SRCDIR)Station.cpp -o $@

$(OBJDIR)Resource.o: $(SRCDIR)Resource.cpp $(INCDIR)Resource.h
	$(CC) $(CFLAGS) $(SRCDIR)Resource.cpp -o $@
	
$(OBJDIR)TrainEvent.o: $(SRCDIR)TrainEvent.cpp $(INCDIR)TrainEvent.h
	$(CC) $(CFLAGS) $(SRCDIR)TrainEvent.cpp -o $@

$(OBJDIR)SolutionRg.o: $(SRCDIR)SolutionRg.cpp $(INCDIR)SolutionRg.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)SolutionRg.cpp -o $@

$(OBJDIR)SolutionR0.o: $(SRCDIR)SolutionR0.cpp $(INCDIR)SolutionR0.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)SolutionR0.cpp -o $@

$(OBJDIR)SolutionRc.o: $(SRCDIR)SolutionRc.cpp $(INCDIR)SolutionRc.h $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)SolutionRc.cpp -o $@

$(OBJDIR)Solution.o: $(SRCDIR)Solution.cpp $(INCDIR)Solution.h
	$(CC) $(CFLAGS) $(SRCDIR)Solution.cpp -o $@

run:
	./$(OBJDIR)simulate $(NETWORK)

clean: 
	rm $(OBJDIR)/*.o 
	rm $(OBJDIR)/simulate