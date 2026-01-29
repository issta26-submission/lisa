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
//<ID> 565
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"config\" : { \"name\" : \"orig\", \"items\" : [\"one\"] }, \"flag\" : false } ";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);
    cJSON *ref_item = cJSON_CreateString("referenced");
    cJSON_AddItemToObject(root, "orig_ref", ref_item);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(arr, ref_item);
    cJSON_AddItemToObject(root, "extra", arr);

    // step 2: Configure
    cJSON *config = cJSON_GetObjectItem(root, "config");
    cJSON *name_item = cJSON_GetObjectItem(config, "name");
    char *newname = cJSON_SetValuestring(name_item, "updated_name");
    (void)newname;
    cJSON_bool has_extra = cJSON_HasObjectItem(root, "extra");
    (void)has_extra;

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 512, 1);
    const char *extracted_ref = cJSON_GetStringValue(cJSON_GetObjectItem(root, "orig_ref"));
    const char *extracted_name = cJSON_GetStringValue(name_item);
    (void)extracted_ref;
    (void)extracted_name;

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}