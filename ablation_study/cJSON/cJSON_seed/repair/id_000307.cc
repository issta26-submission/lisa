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
//<ID> 307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("example"));
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddItemToObject(meta, "note", cJSON_CreateString("data"));

    // step 3: Operate & Validate
    cJSON *ver_item = cJSON_GetObjectItem(root, "version");
    double ver = cJSON_GetNumberValue(ver_item);
    cJSON *count_item = cJSON_GetObjectItem(meta, "count");
    double count = cJSON_GetNumberValue(count_item);
    double computed = ver * count + 0.5;
    cJSON_AddNumberToObject(root, "computed", computed);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *pretty_meta = cJSON_Print(meta);

    // step 4: Cleanup
    cJSON_free(unformatted);
    cJSON_free(pretty_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}