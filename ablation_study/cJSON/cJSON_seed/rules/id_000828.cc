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
//<ID> 828
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *false_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *orig_raw = NULL;
    cJSON *dup_raw = NULL;
    char *printed = NULL;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    double num_value = 0.0;
    cJSON_bool raw_is_string = 0;
    cJSON_bool printed_prealloc = 0;
    cJSON_bool raws_equal = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    raw_item = cJSON_CreateRaw("{\"inner\":\"value\"}");
    cJSON_AddItemToObject(root, "raw", raw_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", false_item);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    num_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "answer"));
    raw_is_string = cJSON_IsString(cJSON_GetObjectItem(root, "raw"));
    printed_prealloc = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    dup_root = cJSON_Duplicate(root, 1);
    orig_raw = cJSON_GetObjectItem(root, "raw");
    dup_raw = cJSON_GetObjectItem(dup_root, "raw");
    raws_equal = cJSON_Compare(orig_raw, dup_raw, 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);
    memset(buffer, 0, sizeof(buffer));

    // API sequence test completed successfully
    (void)num_value;
    (void)raw_is_string;
    (void)printed_prealloc;
    (void)raws_equal;
    return 66;
}