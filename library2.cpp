#include "library2.h"
#include "CourseManager.h"
#include <new>

void* Init() {
	auto* DS = new CourseManager();
	return (void*)DS;
}

StatusType AddCourse(void* DS, int courseID) {
	if (courseID <= 0 || DS == nullptr) {
		return INVALID_INPUT;
	}
	if ((((CourseManager*)DS)->accessCourseData(courseID))) {
		return FAILURE;
	}
	try {
		((CourseManager*)DS)->addCourse(courseID);
	}
	catch (std::bad_alloc & ba) {
		ba.what();
		return ALLOCATION_ERROR;
	}
	return SUCCESS;
}

StatusType RemoveCourse(void* DS, int courseID) {
	if (DS == nullptr || courseID <= 0) {
		return INVALID_INPUT;
	}
	DoubleDirectionNode* res = ((CourseManager*)DS)->accessCourseData(courseID);
	if (!res) {
		return FAILURE;
	}

	((CourseManager*)DS)->RemoveArtist_M(courseID);
	return SUCCESS;
}

StatusType AddClass(void* DS, int courseID, int* classID) {
	if (DS == nullptr || courseID <= 0) {
		return INVALID_INPUT;
	}
	DoubleDirectionNode* res = ((CourseManager*)DS)->accessCourseData(courseID);
if(!res)
    return FAILURE;
	try {
		//int* newRank = new int(0);
		int* newRank=new int (0);

		res->classeshist[res->current_num_of_classes] = res->current_num_of_classes;
		res->listens[res->current_num_of_classes] = 0;
		res->classeshist.updateArr();
		res->listens.updateArr();

*classID=res->current_num_of_classes;
res->current_num_of_classes++;
delete newRank;
     //   WatchClass(DS, courseID,*classID,1);
	}
	catch (std::bad_alloc & ba) {
		ba.what();
		return ALLOCATION_ERROR;
	}

	return SUCCESS;
}

StatusType WatchClass(void *DS, int courseID, int classID, int time){
	if (DS == nullptr || classID< 0 || courseID <= 0 || time<= 0) {
		return INVALID_INPUT;
	}
	DoubleDirectionNode* res = ((CourseManager*)DS)->accessCourseData(courseID);
	if (!res) {
		return FAILURE;
	}

	try {
	   if( classID+1>res->classeshist.current)
       {
           return INVALID_INPUT;
       }
       if(	res->classeshist[classID]!=classID ){
           return FAILURE;
       }
	int oldStreams=res->listens[classID];
		res->listens[classID] += time;
		res->classeshist.updateArr();
		res->listens.updateArr();

        ((CourseManager*)DS)->add_to_RanksTree_with_newRank(courseID,classID,res->listens[classID],oldStreams);
		return SUCCESS;
	}
	catch (std::bad_alloc & ba) {
		ba.what();
		return ALLOCATION_ERROR;
	}
}



StatusType GetIthWatchedClass(void* DS, int i, int* courseID, int* classID) {
	if (!DS || i <= 0 || !courseID|| !classID) {
		return INVALID_INPUT;
	}
    if(!((CourseManager *) DS)->getRanksTree().Root){
        return FAILURE;
    }

	int num_of_nodes=((CourseManager *) DS)->getRanksTree().Root->w+1;
 keyGen* ith = ((CourseManager *) DS)->getRanksTree().select(num_of_nodes-i+1);
	if(ith== nullptr)
        return FAILURE;
	*courseID= (ith->course_id);
	*classID= (ith->class_id);
    return SUCCESS;
}
StatusType TimeViewed(void *DS, int courseID, int classID, int *timeViewed) {
    if (DS == nullptr || classID <0 || courseID <= 0) {
        return INVALID_INPUT;
    }
    DoubleDirectionNode *res = ((CourseManager *) DS)->accessCourseData(courseID);
    if (!res) {
        return FAILURE;
    }

    try {
        if( classID+1>res->classeshist.current)
        {
            return INVALID_INPUT;
        }
        if (res->classeshist[classID] != classID) {
            return FAILURE;
        }
      *timeViewed=res->listens[classID];
        return SUCCESS;
    }
    catch (std::bad_alloc & ba) {
        ba.what();
        return ALLOCATION_ERROR;
    }
}
void Quit(void** DS) {
	delete* (CourseManager**)DS;
	*DS = nullptr;
}
