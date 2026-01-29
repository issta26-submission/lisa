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
//<ID> 1822
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    const char *strval = (const char *)0;
    cJSON_bool add_ok = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("example");

    // step 3: Configure
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    add_ok = cJSON_AddItemToObject(root, "name", str_item);

    // step 4: Operate
    retrieved_name = cJSON_GetObjectItem(root, "name");
    retrieved_num = cJSON_GetObjectItem(root, "count");
    strval = cJSON_GetStringValue(retrieved_name);

    // step 5: Validate
    validation = (int)(retrieved_name != (cJSON *)0) + (int)(retrieved_num != (cJSON *)0) + (int)(strval != (const char *)0) + (int)(num_item != (cJSON *)0) + (int)(add_ok != 0);
    (void)validation;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)root;
    (void)str_item;
    (void)num_item;
    (void)retrieved_name;
    (void)retrieved_num;
    (void)strval;
    (void)add_ok;
    (void)validation;
    // API sequence test completed successfully
    return 66;
}