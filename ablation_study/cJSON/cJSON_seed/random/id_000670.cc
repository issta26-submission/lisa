#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 670
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *numA = (cJSON *)0;
    cJSON *numB = (cJSON *)0;
    cJSON *strInArr = (cJSON *)0;
    cJSON *numObj = (cJSON *)0;
    cJSON *noteObj = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    cJSON_bool addedA = 0;
    cJSON_bool addedB = 0;
    cJSON_bool addedS = 0;
    double answer_val = 0.0;
    char *note_val = (char *)0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    numA = cJSON_CreateNumber(1.5);
    numB = cJSON_CreateNumber(2.5);
    strInArr = cJSON_CreateString("element");
    numObj = cJSON_CreateNumber(42.0);
    noteObj = cJSON_CreateString("TheAnswer");

    // step 3: Configure
    addedA = cJSON_AddItemToArray(arr, numA);
    addedB = cJSON_AddItemToArray(arr, numB);
    addedS = cJSON_AddItemToArray(arr, strInArr);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "answer", numObj);
    cJSON_AddItemToObject(root, "note", noteObj);

    // step 4: Operate
    retrieved_num = cJSON_GetObjectItem(root, "answer");
    answer_val = cJSON_GetNumberValue(retrieved_num);
    retrieved_str = cJSON_GetObjectItem(root, "note");
    note_val = cJSON_GetStringValue(retrieved_str);
    nonnull_count = (note_val != (char *)0) + (answer_val != 0.0);

    // step 5: Validate
    (void)addedA;
    (void)addedB;
    (void)addedS;
    (void)retrieved_num;
    (void)retrieved_str;
    (void)answer_val;
    (void)note_val;
    (void)nonnull_count;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}