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
//<ID> 904
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *null_node = cJSON_CreateNull();
    cJSON *flag_node = cJSON_AddBoolToObject(root, "flag", 1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "missing", null_node);
    cJSON_AddStringToObject(root, "id", "sensor-1");

    // step 3: Operate & Validate
    char json_buf[] = " { \"flag\": true, \"missing\": null, \"id\": \"sensor-1\" } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON_AddNumberToObject(root, "equal", (double)equal);
    char *snapshot = cJSON_PrintUnformatted(parsed);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}