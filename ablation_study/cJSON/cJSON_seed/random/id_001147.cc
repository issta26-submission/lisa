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
//<ID> 1147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *values = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *name_str_item = (cJSON *)0;
    cJSON *fetched_meta = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    cJSON *fetched_values = (cJSON *)0;
    cJSON *fetched_elem1 = (cJSON *)0;
    cJSON *detached_elem = (cJSON *)0;
    char *name_cstr = (char *)0;
    double detached_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root, sub-object and array
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "values", values);

    // step 3: Configure - populate object and array with items (string and numbers)
    name_str_item = cJSON_CreateString("example-name");
    cJSON_AddItemToObject(meta, "name", name_str_item);
    num0 = cJSON_CreateNumber(42.0);
    num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(values, num0);
    cJSON_AddItemToArray(values, num1);

    // step 4: Operate - retrieve object members and array elements, detach an element
    fetched_meta = cJSON_GetObjectItem(root, "meta");
    fetched_name = cJSON_GetObjectItem(fetched_meta, "name");
    name_cstr = cJSON_GetStringValue(fetched_name);
    fetched_values = cJSON_GetObjectItem(root, "values");
    fetched_elem1 = cJSON_GetArrayItem(fetched_values, 1);
    detached_elem = cJSON_DetachItemViaPointer(fetched_values, fetched_elem1);
    detached_value = cJSON_GetNumberValue(detached_elem);

    // step 5: Validate - compute a simple validation score from extracted data
    validation_score = (int)(name_cstr != (char *)0) + (int)(detached_value == 3.14);
    (void)validation_score;

    // step 6: Cleanup - delete detached item and root (which frees remaining children)
    cJSON_Delete(detached_elem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}