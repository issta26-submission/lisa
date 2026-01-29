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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(0.0);
    const int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    double before_value = 0.0;
    double set_result = 0.0;
    double after_value = 0.0;
    const char *label_value = NULL;
    cJSON_bool print_ok = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "value", num);
    cJSON_AddStringToObject(root, "label", "demo");

    // step 3: Operate / Validate
    before_value = cJSON_GetNumberValue(num);
    set_result = cJSON_SetNumberHelper(num, 123.456);
    after_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "value"));
    label_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "label"));
    print_ok = cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    (void)before_value;
    (void)set_result;
    (void)after_value;
    (void)label_value;
    (void)print_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}