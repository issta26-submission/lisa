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
//<ID> 1700
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
    cJSON *retrieved = (cJSON *)0;
    char *unformatted = (char *)0;
    double extracted_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(3.1415);

    // step 3: Configure
    cJSON_AddItemToObject(root, "pi", num_item);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "pi");
    extracted_value = cJSON_GetNumberValue(retrieved);
    unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(extracted_value == 3.1415) + (int)(unformatted != (char *)0);
    (void)validation;

    // step 6: Cleanup
    if (unformatted) { cJSON_free(unformatted); }
    cJSON_Delete(root);
    (void)root;
    (void)num_item;
    (void)retrieved;
    (void)extracted_value;
    (void)unformatted;
    // API sequence test completed successfully
    return 66;
}