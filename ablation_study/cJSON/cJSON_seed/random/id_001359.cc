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
//<ID> 1359
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *dup_root = (cJSON *)0;
    cJSON *dup_val = (cJSON *)0;
    cJSON *dup_greet = (cJSON *)0;
    const char *orig_str = (const char *)0;
    const char *dup_str = (const char *)0;
    double extracted_num = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create a root object and a numeric item
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.0);

    // step 3: Configure - attach the numeric item and a string to the root object
    cJSON_AddItemToObject(root, "value", num_item);
    str_item = cJSON_AddStringToObject(root, "greeting", "hello-world");

    // step 4: Operate - duplicate the root (deep copy) and extract duplicated members
    dup_root = cJSON_Duplicate(root, 1); // recurse = true
    dup_val = cJSON_GetObjectItem(dup_root, "value");
    dup_greet = cJSON_GetObjectItem(dup_root, "greeting");
    extracted_num = cJSON_GetNumberValue(dup_val);
    orig_str = cJSON_GetStringValue(str_item);
    dup_str = cJSON_GetStringValue(dup_greet);

    // step 5: Validate - simple consistency checks aggregated into a score
    validation_score = (int)(root != (cJSON *)0) + (int)(dup_root != (cJSON *)0) + (int)(extracted_num == 123.0) + (int)(orig_str != (const char *)0) + (int)(dup_str != (const char *)0);

    // step 6: Cleanup - free both original and duplicated structures
    cJSON_Delete(root);
    cJSON_Delete(dup_root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)orig_str;
    (void)dup_str;
    (void)extracted_num;
    return 66;
}