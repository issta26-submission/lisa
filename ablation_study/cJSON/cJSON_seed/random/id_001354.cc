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
//<ID> 1354
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *str = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    const char *retrieved = (const char *)0;
    double extracted_num = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object, a numeric item and a string item
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.0);
    str = cJSON_CreateString("hello world");

    // step 3: Configure - attach the number and the original string to the root object
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddItemToObject(root, "greeting", str);

    // step 4: Operate - duplicate the string item and retrieve its C string value
    dup = cJSON_Duplicate(str, 1);
    cJSON_AddItemToObject(root, "greeting_dup", dup);
    retrieved = cJSON_GetStringValue(dup);

    // step 5: Validate / augment - add a new string to the object using the retrieved value and read back the number
    cJSON_AddStringToObject(root, "greeting_copy", retrieved);
    extracted_num = cJSON_GetNumberValue(num);
    validation_score = (int)(retrieved != (const char *)0) + (int)(cJSON_GetStringValue(str) != (const char *)0) + (int)(extracted_num == 42.0);

    // step 6: Cleanup - delete the root (frees attached items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)retrieved;
    (void)extracted_num;
    return 66;
}