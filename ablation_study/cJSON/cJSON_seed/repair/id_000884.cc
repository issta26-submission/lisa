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
//<ID> 884
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
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *name = cJSON_CreateString("cjson_demo");
    cJSON_AddItemToObject(meta, "name", name);

    // step 2: Configure
    cJSON *shared_number = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", shared_number);
    cJSON_AddItemReferenceToObject(meta, "alias_answer", shared_number);

    // step 3: Operate and Validate
    const char *name_str = cJSON_GetStringValue(name);
    double name_ascii = (double)(unsigned char)name_str[0];
    cJSON_AddNumberToObject(root, "name_ascii", name_ascii);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}