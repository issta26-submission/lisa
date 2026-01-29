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
//<ID> 868
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *parsed = (cJSON *)0;
    cJSON *elem = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    const char *json = "[10.5, 20.25, 30.75]";
    size_t len = 0;
    const char *endptr = (const char *)0;
    double value = 0.0;
    cJSON_bool added_parsed = 0;
    cJSON_bool added_num = 0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    len = (size_t)strlen(json);
    parsed = cJSON_ParseWithLengthOpts(json, len, &endptr, 1);
    root = cJSON_CreateObject();

    // step 3: Operate (extract an element and derive a number)
    elem = cJSON_GetArrayItem(parsed, 1);
    value = cJSON_GetNumberValue(elem);
    num = cJSON_CreateNumber(value);

    // step 4: Configure (attach parsed array and created number to object)
    added_parsed = cJSON_AddItemToObject(root, "values", parsed);
    added_num = cJSON_AddItemToObject(root, "picked", num);

    // step 5: Validate (aggregate simple checks)
    validation_score = (int)added_parsed + (int)added_num + (root != (cJSON *)0) + (elem != (cJSON *)0) + (num != (cJSON *)0) + (endptr != (const char *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}