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
//<ID> 1180
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *orig_num = NULL;
    cJSON *got_item = NULL;
    cJSON *new_num = NULL;
    cJSON *status_true = NULL;
    cJSON *orphan = NULL;
    cJSON_bool replaced = 0;
    double value_before = 0.0;
    double value_after = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    orig_num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "value", orig_num);
    status_true = cJSON_AddTrueToObject(root, "ok");
    orphan = cJSON_CreateString("temporary_orphan");

    // step 3: Operate / Validate
    got_item = cJSON_GetObjectItem(root, "value");
    value_before = cJSON_GetNumberValue(got_item);
    new_num = cJSON_CreateNumber(456.0);
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, new_num);
    got_item = cJSON_GetObjectItem(root, "value");
    value_after = cJSON_GetNumberValue(got_item);
    (void)status_true;
    (void)replaced;
    (void)value_before;
    (void)value_after;

    // step 4: Cleanup
    cJSON_Delete(orphan);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}