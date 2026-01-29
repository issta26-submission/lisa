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
//<ID> 1140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    const char *names[] = {"Alice", "Bob", "Carol"};
    cJSON *names_arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", names_arr);
    cJSON *numbers = cJSON_CreateObject();
    cJSON_AddNumberToObject(numbers, "one", 1.0);
    cJSON_AddNumberToObject(numbers, "two", 2.0);
    cJSON_AddItemToObject(root, "numbers", numbers);
    /* Use a deep duplicate instead of an object reference to avoid double-free during deletion */
    cJSON *numbers_ref = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(root, "numbers_ref", numbers_ref);
    cJSON *item_two = cJSON_GetObjectItem(numbers, "two");
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(numbers, item_two, replacement);
    cJSON_DeleteItemFromObject(root, "names");
    cJSON *one_item = cJSON_GetObjectItem(numbers, "one");
    double v1 = cJSON_GetNumberValue(one_item);
    cJSON *two_item = cJSON_GetObjectItem(numbers, "two");
    double v2 = cJSON_GetNumberValue(two_item);
    double sum = v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    buf[0] = '\0';
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);
    (void)replaced;
    return 66;
}