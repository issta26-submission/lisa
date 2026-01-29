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
//<ID> 553
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *true_item = cJSON_CreateTrue();
    const char *name_literal = "Charlie";
    cJSON *str_ref = cJSON_CreateStringReference(name_literal);
    cJSON *num_item = NULL;
    cJSON *name_item = NULL;
    char *name_value = NULL;
    double count_value = 0.0;
    char first_char = '\0';
    double combined = 0.0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "flag", true_item);
    num_item = cJSON_AddNumberToObject(root, "count", 256.75);
    cJSON_AddItemToObject(root, "name", str_ref);

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(name_item);
    count_value = cJSON_GetNumberValue(num_item);
    first_char = name_value[0];
    combined = count_value + (double)first_char;
    (void)combined;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}