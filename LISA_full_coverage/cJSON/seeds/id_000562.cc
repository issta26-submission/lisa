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
//<ID> 562
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char raw_json[] = " { \"items\": [\"one\",\"two\"], \"info\": { \"name\": \"orig\" } } ";
    size_t raw_len = sizeof(raw_json);
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(mutable_json, raw_len, &parse_end, 1);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *ref_item = cJSON_CreateStringReference("external");
    cJSON_AddItemReferenceToArray(items, ref_item);
    cJSON *info = cJSON_GetObjectItem(root, "info");
    cJSON *name_item = cJSON_GetObjectItem(info, "name");
    char *sv = cJSON_SetValuestring(name_item, "modified");
    (void)sv;

    // step 3: Operate & Validate
    cJSON_bool has_info = cJSON_HasObjectItem(root, "info");
    char *printed = cJSON_PrintBuffered(root, 64, 1);
    const char *extracted_name = cJSON_GetStringValue(name_item);
    (void)has_info;
    (void)extracted_name;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}