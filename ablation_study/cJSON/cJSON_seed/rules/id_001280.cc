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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *mirror = NULL;
    cJSON *num_item = NULL;
    cJSON *null_item = NULL;
    cJSON *retrieved = NULL;
    cJSON *detached = NULL;
    char *printed = NULL;
    double number_value = 0.0;
    cJSON_bool added_ref = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(123.456);
    cJSON_AddItemToObject(root, "number", num_item);
    null_item = cJSON_AddNullToObject(root, "nothing");
    mirror = cJSON_CreateObject();
    added_ref = cJSON_AddItemReferenceToObject(mirror, "number_ref", num_item);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "number");
    number_value = cJSON_GetNumberValue(retrieved);
    printed = cJSON_PrintUnformatted(root);
    detached = cJSON_DetachItemViaPointer(mirror, cJSON_GetObjectItem(mirror, "number_ref"));
    (void)added_ref;
    (void)detached;
    (void)number_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(mirror);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}