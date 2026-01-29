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
//<ID> 364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *topnum = NULL;
    cJSON *innum = NULL;
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_innum = NULL;
    double innum_value = 0.0;

    // step 2: Setup / Configure
    topnum = cJSON_AddNumberToObject(root, "topnum", 1.23);
    cJSON_AddItemToObject(root, "child", child);
    innum = cJSON_AddNumberToObject(child, "innum", 7.0);
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);

    // step 3: Operate / Validate
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "topnum");
    retrieved_child = cJSON_GetObjectItem(parsed, "child");
    retrieved_innum = cJSON_GetObjectItem(retrieved_child, "innum");
    innum_value = cJSON_GetNumberValue(retrieved_innum);
    buffer[0] = (char)('0' + (int)innum_value);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}