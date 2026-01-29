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
//<ID> 556
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *label = "example_label";
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *str_ref = cJSON_CreateStringReference(label);
    cJSON *num_item = NULL;
    const cJSON *retrieved_label_item = NULL;
    char *label_value = NULL;
    char *printed = NULL;
    double count_val = 0.0;
    double combined = 0.0;
    char first_char = '\0';

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "label", str_ref);
    num_item = cJSON_AddNumberToObject(root, "count", 256.0);

    // step 3: Operate / Validate
    retrieved_label_item = cJSON_GetObjectItem(root, "label");
    label_value = cJSON_GetStringValue(retrieved_label_item);
    printed = cJSON_PrintUnformatted(root);
    count_val = cJSON_GetNumberValue(num_item);
    first_char = label_value[0];
    combined = count_val + (double)first_char;
    (void)combined;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}