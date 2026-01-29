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
//<ID> 557
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *label_ref = cJSON_CreateStringReference("example");
    cJSON *num_item = NULL;
    char *printed = NULL;
    char *label_str = NULL;
    double count_value = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "active", true_item);
    num_item = cJSON_AddNumberToObject(root, "count", 42.0);
    cJSON_AddItemToObject(root, "label", label_ref);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    label_str = cJSON_GetStringValue(label_ref);
    buffer[0] = label_str ? label_str[0] : '\0';
    buffer[1] = printed ? printed[0] : '\0';
    count_value = cJSON_GetNumberValue(num_item);
    buffer[2] = (char)((int)count_value % 256);
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}