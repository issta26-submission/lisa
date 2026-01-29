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
//<ID> 1577
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"value\":3.14,\"nested\":{\"value\":3.14}}";
    cJSON *root = NULL;
    cJSON *dup_root = NULL;
    cJSON *val_item = NULL;
    cJSON *dup_val_item = NULL;
    cJSON *nested = NULL;
    char *printed = NULL;
    double num_val = 0.0;
    double dup_num_val = 0.0;
    cJSON_bool cmp = 0;

    // step 2: Setup / Configure
    root = cJSON_Parse(json);
    val_item = cJSON_GetObjectItem(root, "value");
    nested = cJSON_GetObjectItem(root, "nested");
    printed = cJSON_PrintUnformatted(root);
    dup_root = cJSON_Parse(printed);
    dup_val_item = cJSON_GetObjectItem(dup_root, "value");

    // step 3: Operate / Validate
    num_val = cJSON_GetNumberValue(val_item);
    dup_num_val = cJSON_GetNumberValue(dup_val_item);
    cmp = cJSON_Compare(root, dup_root, 1);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}