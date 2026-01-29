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
//<ID> 713
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *temp_buffer = (char *)cJSON_malloc(256);
    memset(temp_buffer, 0, 256);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = NULL;
    cJSON *num_root = NULL;
    cJSON *num_child = NULL;
    cJSON *flag_item = NULL;
    cJSON *true_item = NULL;
    double child_number_value = 0.0;
    cJSON_bool flag_is_true = 0;

    // step 2: Setup / Configure
    num_root = cJSON_AddNumberToObject(root, "root_value", 42.0);
    child = cJSON_AddObjectToObject(root, "child"); 
    num_child = cJSON_AddNumberToObject(child, "inner", 3.14159);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate / Validate
    flag_item = cJSON_GetObjectItem(root, "enabled");
    flag_is_true = cJSON_IsTrue(flag_item);
    cJSON *got_inner = cJSON_GetObjectItem(child, "inner");
    child_number_value = cJSON_GetNumberValue(got_inner);
    memcpy(temp_buffer, "child:", 6);
    /* store a textual representation of the number into the buffer (simple, no snprintf) */
    {
        /* create small temporary string using malloc to avoid using snprintf/printf */
        char *num_str = (char *)cJSON_malloc(64);
        memset(num_str, 0, 64);
        double v = child_number_value;
        /* crude conversion: handle typical small positive number representation */
        int int_part = (int)v;
        int frac = (int)((v - int_part) * 1000000.0);
        if (frac < 0) frac = -frac;
        /* build the string manually */
        char tmp[64];
        memset(tmp, 0, 64);
        /* simple integer to string for int_part */
        int idx = 0;
        int ip = int_part;
        if (ip == 0) {
            tmp[idx++] = '0';
        } else {
            int rev = 0;
            while (ip > 0) {
                rev = rev * 10 + (ip % 10);
                ip = ip / 10;
            }
            while (rev > 0) {
                tmp[idx++] = '0' + (rev % 10);
                rev = rev / 10;
            }
        }
        tmp[idx++] = '.';
        int j;
        int pw = 100000;
        for (j = 0; j < 6; ++j) {
            int d = frac / pw;
            tmp[idx++] = '0' + d;
            frac = frac % pw;
            pw = pw / 10;
        }
        tmp[idx] = '\0';
        memcpy(num_str, tmp, strlen(tmp));
        memcpy(temp_buffer + 6, num_str, strlen(num_str));
        temp_buffer[6 + strlen(num_str)] = '\0';
        cJSON_free(num_str);
    }

    // step 4: Cleanup
    cJSON_free(temp_buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}