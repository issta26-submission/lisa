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
//<ID> 530
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *darray = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *title_value = (char *)0;
    cJSON_bool is_raw = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    double numbers[4] = {1.5, 2.5, 3.5, 4.5};

    // step 2: Initialize
    root = cJSON_CreateObject();
    darray = cJSON_CreateDoubleArray(numbers, 4);
    str_item = cJSON_CreateString("extra_label");
    raw_item = cJSON_CreateRaw("unparsed:raw_data");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(darray, str_item);
    add_ok2 = cJSON_AddItemToArray(darray, raw_item);
    cJSON_AddItemToObject(root, "measurements", darray);
    cJSON_AddItemToObject(root, "title", cJSON_CreateString("sensor_readings"));

    // step 4: Operate
    retrieved_array = cJSON_GetObjectItem(root, "measurements");
    first_elem = cJSON_GetArrayItem(retrieved_array, 0);
    is_raw = cJSON_IsRaw(first_elem);
    title_value = cJSON_GetStringValue(cJSON_GetObjectItem(root, "title"));

    // step 5: Validate
    (void)is_raw;
    (void)title_value;
    (void)add_ok1;
    (void)add_ok2;
    (void)first_elem;
    (void)retrieved_array;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}