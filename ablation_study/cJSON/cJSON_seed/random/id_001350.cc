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
//<ID> 1350
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *orig_container = (cJSON *)0;
    cJSON *dup_container = (cJSON *)0;
    cJSON *greeting_orig = (cJSON *)0;
    cJSON *greeting_dup = (cJSON *)0;
    cJSON *answer_dup = (cJSON *)0;
    const char *str_orig = (const char *)0;
    const char *str_dup = (const char *)0;
    double dup_answer_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root and a child object with a number and a string
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", num);
    cJSON_AddStringToObject(child, "greeting", "hello");

    // step 3: Configure - attach the original child to root and create a deep duplicate then attach it
    cJSON_AddItemToObject(root, "original", child);
    dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "duplicate", dup);

    // step 4: Operate - retrieve string values from original and duplicate, and the numeric value from duplicate
    orig_container = cJSON_GetObjectItem(root, "original");
    greeting_orig = cJSON_GetObjectItem(orig_container, "greeting");
    str_orig = cJSON_GetStringValue(greeting_orig);
    dup_container = cJSON_GetObjectItem(root, "duplicate");
    greeting_dup = cJSON_GetObjectItem(dup_container, "greeting");
    str_dup = cJSON_GetStringValue(greeting_dup);
    answer_dup = cJSON_GetObjectItem(dup_container, "answer");
    dup_answer_val = cJSON_GetNumberValue(answer_dup);

    // step 5: Validate - compute a simple validation score from observed values
    validation_score = (int)(str_orig != (const char *)0) + (int)(str_dup != (const char *)0) + (int)(dup_answer_val == 42.0);

    // step 6: Cleanup - delete the root which frees attached items including the duplicate
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)str_orig;
    (void)str_dup;
    (void)dup_answer_val;
    return 66;
}