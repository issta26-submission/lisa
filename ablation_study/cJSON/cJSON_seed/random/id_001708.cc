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
//<ID> 1708
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
    cJSON *num = (cJSON *)0;
    char *unformatted = (char *)0;
    double extracted_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    num = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    cJSON_AddItemToObject(child, "pi", num);
    cJSON_AddItemToObject(root, "data", child);

    // step 4: Operate
    unformatted = cJSON_PrintUnformatted(root);
    extracted_value = cJSON_GetNumberValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "data"), "pi"));

    // step 5: Validate
    validation = (int)(unformatted != (char *)0) + (int)(extracted_value == 3.1415);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(root);
    (void)root;
    (void)child;
    (void)num;
    (void)extracted_value;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}