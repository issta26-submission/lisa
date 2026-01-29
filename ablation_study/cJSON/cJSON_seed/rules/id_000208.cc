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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *str = cJSON_CreateString("example");
    cJSON *flag = NULL;
    double original_value = 0.0;
    double after_set_value = 0.0;
    double set_return = 0.0;
    cJSON_bool printed_ok = 0;
    cJSON_bool flag_is_false = 0;
    const char *text_value = NULL;
    const int buf_len = 256;
    char *print_buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(print_buf, 0, buf_len);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "number", num);
    cJSON_AddItemToObject(root, "text", str);
    flag = cJSON_AddFalseToObject(root, "flag");

    // step 3: Operate / Validate
    original_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    set_return = cJSON_SetNumberHelper(num, 42.5);
    after_set_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "number"));
    printed_ok = cJSON_PrintPreallocated(root, print_buf, buf_len, 0);
    flag_is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag"));
    text_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "text"));
    (void)original_value;
    (void)set_return;
    (void)after_set_value;
    (void)printed_ok;
    (void)flag_is_false;
    (void)text_value;
    (void)flag;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(print_buf);

    // API sequence test completed successfully
    return 66;
}