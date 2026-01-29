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
//<ID> 1165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *flags = (cJSON *)0;
    cJSON *b_true = (cJSON *)0;
    cJSON *b_false = (cJSON *)0;
    cJSON *top_flag = (cJSON *)0;
    cJSON *fetched_nested = (cJSON *)0;
    cJSON *fetched_flags = (cJSON *)0;
    cJSON *elem0 = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root and a nested object, attach nested to root
    root = cJSON_CreateObject();
    nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);

    // step 3: Configure - create an array under nested and populate with booleans and a top-level bool
    flags = cJSON_CreateArray();
    cJSON_AddItemToObject(nested, "flags", flags);
    b_true = cJSON_CreateTrue();
    b_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(flags, b_true);
    cJSON_AddItemToArray(flags, b_false);
    top_flag = cJSON_CreateBool(0);
    cJSON_AddItemToObject(root, "top_flag", top_flag);

    // step 4: Operate - retrieve nested structures and array elements
    fetched_nested = cJSON_GetObjectItem(root, "nested");
    fetched_flags = cJSON_GetObjectItem(fetched_nested, "flags");
    elem0 = cJSON_GetArrayItem(fetched_flags, 0);
    elem1 = cJSON_GetArrayItem(fetched_flags, 1);

    // step 5: Validate - inspect types using cJSON_IsBool and perform a simple accumulation
    validation_score = (int)cJSON_IsBool(elem0) + (int)cJSON_IsBool(elem1) + (int)(fetched_nested != (cJSON *)0) + (int)(fetched_flags != (cJSON *)0);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}