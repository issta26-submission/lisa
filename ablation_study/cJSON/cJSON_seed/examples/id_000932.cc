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
//<ID> 932
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metadata = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"vendor\":\"ACME\",\"version\":1}");
    cJSON *name = cJSON_CreateString("sensor-A");
    cJSON *dup_name = cJSON_Duplicate(name, 1);

    // step 2: Configure
    cJSON_AddItemToObject(metadata, "name", dup_name);
    cJSON_AddItemToObject(metadata, "raw", raw);
    cJSON_AddItemToObject(root, "metadata", metadata);
    cJSON *pi_val = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "pi", pi_val);

    // step 3: Operate & Validate
    cJSON *got_meta = cJSON_GetObjectItem(root, "metadata");
    cJSON *got_name = cJSON_GetObjectItem(got_meta, "name");
    cJSON_bool is_str = cJSON_IsString(got_name);
    const char *text = cJSON_GetStringValue(got_name);
    cJSON *metadata_dup = cJSON_Duplicate(metadata, 1);
    cJSON_AddItemToObject(root, "metadata_dup", metadata_dup);
    (void)is_str;
    (void)text;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(name);

    return 66;
    // API sequence test completed successfully
}