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
//<ID> 1828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_count = (cJSON *)0;
    const char *name_str = (const char *)0;
    double count_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");

    // step 3: Configure
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddNumberToObject(root, "count", 42.0);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(retrieved_name);
    retrieved_count = cJSON_GetObjectItem(root, "count");
    count_value = cJSON_GetNumberValue(retrieved_count);

    // step 5: Validate
    validation = (int)(root != (cJSON *)0) + (int)(retrieved_name != (cJSON *)0) + (int)(name_str != (const char *)0) + (int)(retrieved_count != (cJSON *)0) + (int)(count_value == 42.0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)name_item;
    (void)retrieved_name;
    (void)retrieved_count;
    (void)name_str;
    (void)count_value;

    // API sequence test completed successfully
    return 66;
}