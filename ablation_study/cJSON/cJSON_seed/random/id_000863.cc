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
//<ID> 863
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char json[] = "[1,2,3]";
    const char * parse_end = (const char *)0;
    cJSON * parsed = (cJSON *)0;
    cJSON * root = (cJSON *)0;
    cJSON * item0 = (cJSON *)0;
    cJSON * item1 = (cJSON *)0;
    cJSON * item2 = (cJSON *)0;
    cJSON * sumnum = (cJSON *)0;
    cJSON_bool added_parsed = 0;
    cJSON_bool added_sum = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    int validation = 0;

    // step 2: Initialize / Parse input with explicit length and null-terminated requirement
    parsed = cJSON_ParseWithLengthOpts(json, (size_t)7, &parse_end, (cJSON_bool)1);

    // step 3: Operate - extract values from the parsed array
    item0 = cJSON_GetArrayItem(parsed, 0);
    item1 = cJSON_GetArrayItem(parsed, 1);
    v0 = cJSON_GetNumberValue(item0);
    v1 = cJSON_GetNumberValue(item1);
    sum = v0 + v1;

    // step 4: Configure - create an object, create a sum number and add items into the object
    root = cJSON_CreateObject();
    sumnum = cJSON_CreateNumber(sum);
    added_parsed = cJSON_AddItemToObject(root, "numbers", parsed);
    added_sum = cJSON_AddItemToObject(root, "sum", sumnum);

    // step 5: Validate - access another array element and compute a simple validation score
    item2 = cJSON_GetArrayItem(parsed, 2);
    v2 = cJSON_GetNumberValue(item2);
    validation = (int)(item0 != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)added_parsed + (int)added_sum + (int)(v2 == 3.0) + (int)(sum == (v0 + v1));
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}