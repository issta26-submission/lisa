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
//<ID> 1454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *copy_item = NULL;
    cJSON *str_ref = NULL;
    cJSON *true_item = NULL;
    char prebuffer[256];
    cJSON_bool print_ok = 0;
    cJSON_bool is_true_flag = 0;
    double value = 0.0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);
    str_ref = cJSON_CreateStringReference("a referenced string");
    cJSON_AddItemToObject(root, "refstr", str_ref);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate / Validate
    value = cJSON_GetNumberValue(num_item);
    copy_item = cJSON_AddNumberToObject(root, "copy", value);
    memset(prebuffer, 0, sizeof(prebuffer));
    print_ok = cJSON_PrintPreallocated(root, prebuffer, (int)sizeof(prebuffer), 1);
    is_true_flag = cJSON_IsTrue(true_item);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)copy_item;
    (void)print_ok;
    (void)is_true_flag;
    // API sequence test completed successfully
    return 66;
}