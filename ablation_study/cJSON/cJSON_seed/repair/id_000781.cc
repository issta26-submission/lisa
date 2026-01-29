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
//<ID> 781
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"data\":{\"item\":\"value\"},\"num\":3.14}";
    cJSON *doc = cJSON_Parse(json);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddStringToObject(root, "source", "parser");
    cJSON_AddNumberToObject(root, "initial_flag", 1.0);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(doc, "data");
    cJSON_AddItemToObject(root, "data_detached", detached);
    cJSON *numItem = cJSON_GetObjectItem(doc, "num");
    double extracted = cJSON_GetNumberValue(numItem);
    cJSON_AddNumberToObject(root, "extracted_num", extracted);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(doc);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}