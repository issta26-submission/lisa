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
//<ID> 1195
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddStringToObject(root, "name", "tester");
    cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *numitem = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(numitem, 123.25);
    cJSON_AddItemToObject(config, "threshold", numitem);
    const char json[] = "{\"parsed_root\":{\"info\":\"from_parse\"},\"value\":7}";
    size_t len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json, len, &parse_end, 0);
    cJSON_AddStringToObject(parsed, "ingested_by", "parser_v1");

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemViaPointer(root, config);
    cJSON_AddItemToObject(parsed, "reconfig", detached);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}