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
//<ID> 1137
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
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *note = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_elem = (cJSON *)0;
    char *elem_value = (char *)0;
    char *printed = (char *)0;
    cJSON_bool add_arr_ok = (cJSON_bool)0;
    cJSON_bool add_note_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - populate array and add members to root using cJSON_AddItemToObject
    str0 = cJSON_CreateString("hello");
    str1 = cJSON_CreateString("world");
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    add_arr_ok = cJSON_AddItemToObject(root, "greetings", arr);
    note = cJSON_CreateString("example note");
    add_note_ok = cJSON_AddItemToObject(root, "note", note);

    // step 4: Operate - retrieve object member and array element, extract string value, serialize
    fetched_arr = cJSON_GetObjectItem(root, "greetings");
    fetched_elem = cJSON_GetArrayItem(fetched_arr, 1);
    elem_value = cJSON_GetStringValue(fetched_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate - use returned pointers and flags to compute a simple score and free printed buffer
    validation_score = (int)(root != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(fetched_elem != (cJSON *)0) + (int)(elem_value != (char *)0) + (int)add_arr_ok + (int)add_note_ok;
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}