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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *printed = NULL;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *parsed = NULL;
    cJSON *pi_item = NULL;
    cJSON *added_item = NULL;
    double pi_val = 0.0;
    double added_val = 0.0;

    // step 2: Setup / Configure
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    cJSON_AddItemToObject(child, "subnum", cJSON_CreateNumber(42));
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    pi_item = cJSON_GetObjectItem(parsed, "pi");
    pi_val = cJSON_GetNumberValue(pi_item);
    cJSON_AddNumberToObject(parsed, "added_after_parse", pi_val * 2.0);
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "pi");
    added_item = cJSON_GetObjectItem(parsed, "added_after_parse");
    added_val = cJSON_GetNumberValue(added_item);
    buffer[0] = (char)('0' + (int)added_val);
    buffer[1] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}