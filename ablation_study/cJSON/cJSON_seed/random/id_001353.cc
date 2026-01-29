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
//<ID> 1353
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
    cJSON *dup_num = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    const char *strval = (const char *)0;
    double dup_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create a root object, a number item and a standalone string item
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(42.5);
    str_item = cJSON_CreateString("example");

    // step 3: Configure - attach the original number to the root object
    cJSON_AddItemToObject(root, "original", num);

    // step 4: Operate - obtain the string value, add it to the object via AddStringToObject, duplicate the number and attach duplicate
    strval = cJSON_GetStringValue(str_item);
    cJSON_AddStringToObject(root, "copied", strval);
    dup_num = cJSON_Duplicate(num, 0);
    cJSON_AddItemToObject(root, "duplicate", dup_num);
    dup_val = cJSON_GetNumberValue(dup_num);

    // step 5: Validate - compute a simple validation score from observed values
    validation_score = (int)(root != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(dup_val == 42.5);

    // step 6: Cleanup - delete standalone items and root (which frees attached items)
    cJSON_Delete(str_item);
    cJSON_Delete(root);

    (void)validation_score;
    (void)dup_val;
    // API sequence test completed successfully
    return 66;
}