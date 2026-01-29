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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(0.0);
    cJSON *label = cJSON_CreateString("example");
    double read_value = 0.0;
    const char *read_label = NULL;
    char *serialized = NULL;
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddItemToObject(settings, "label", label);
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddFalseToObject(root, "enabled");
    cJSON_SetNumberHelper(num, 123.456);

    // step 3: Operate / Validate
    serialized = cJSON_PrintUnformatted(root);
    ((char *)buffer)[0] = serialized[0];
    read_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    read_label = cJSON_GetStringValue(cJSON_GetObjectItem(settings, "label"));
    (void)read_value;
    (void)read_label;

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);
    cJSON_free(buffer);

    // API sequence test completed successfully
    return 66;
}