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
//<ID> 908
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *flags = NULL;
    cJSON *f0 = NULL;
    cJSON *f1 = NULL;
    cJSON *f2 = NULL;
    cJSON *val_item = NULL;
    const char *json_text = "{\"flags\": [true, false, true], \"value\": 42}";
    cJSON_bool is_f0 = 0;
    cJSON_bool is_f1 = 0;
    cJSON_bool is_f2 = 0;
    double numeric_value = 0.0;
    char *print_buffer = NULL;
    const int buf_len = 128;

    // step 2: Setup / Configure
    print_buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(print_buffer, 0, buf_len);
    parsed = cJSON_Parse(json_text);
    flags = cJSON_GetObjectItem(parsed, "flags");
    f0 = cJSON_GetArrayItem(flags, 0);
    f1 = cJSON_GetArrayItem(flags, 1);
    f2 = cJSON_GetArrayItem(flags, 2);
    val_item = cJSON_GetObjectItem(parsed, "value");

    // step 3: Operate / Validate
    is_f0 = cJSON_IsTrue(f0);
    is_f1 = cJSON_IsTrue(f1);
    is_f2 = cJSON_IsTrue(f2);
    numeric_value = cJSON_GetNumberValue(val_item);
    (void)cJSON_PrintPreallocated(parsed, print_buffer, buf_len, 0);
    print_buffer[0] = (char)('0' + (is_f0 != 0));
    print_buffer[1] = (char)('0' + (is_f1 != 0));
    print_buffer[2] = (char)('0' + (is_f2 != 0));
    print_buffer[3] = (char)('0' + ((int)numeric_value % 10));

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_free(print_buffer);

    // API sequence test completed successfully
    (void)is_f0;
    (void)is_f1;
    (void)is_f2;
    (void)numeric_value;
    return 66;
}