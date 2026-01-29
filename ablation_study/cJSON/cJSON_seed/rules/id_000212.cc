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
//<ID> 212
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.5);
    cJSON *label = cJSON_CreateString("example");
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    double before = cJSON_GetNumberValue(num);
    double new_value = 42.42;
    double set_ret = 0.0;
    double after = 0.0;
    cJSON_bool printed_ok = 0;
    const char *label_val = NULL;
    cJSON *pi_item = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "pi", num);
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate / Validate
    set_ret = cJSON_SetNumberHelper(num, new_value);
    pi_item = cJSON_GetObjectItem(root, "pi");
    after = cJSON_GetNumberValue(pi_item);
    printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);
    buffer[0] = (char)('0' + printed_ok);
    label_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    buffer[1] = label_val[0];

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}